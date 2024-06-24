#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <json-c/json.h>

void process_json_file(const char *file_path, FILE *output_file)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        perror("fopen() error");
        return;
    }

    // moves the pointer to the end of the file
    fseek(file, 0, SEEK_END);
    // gets the length of the file
    long length = ftell(file);
    // moves the pointer to the beginning of the file to read it later
    fseek(file, 0, SEEK_SET);

    // +1 for null terminator since null terminators indicate the end of a valid string
    char *content = malloc(length + 1);
    if (!content)
    {
        perror("malloc() error");
        fclose(file);
        return;
    }

    // Read file content into memory
    fread(content, 1, length, file);
    content[length] = '\0'; // Null-terminate the string
    fclose(file);

    // Parse the JSON content as an array
    struct json_object *parsed_json = json_tokener_parse(content);
    // free the allocated memory
    free(content);

    if (parsed_json == NULL)
    {
        fprintf(stderr, "Failed to parse JSON\n");
        return;
    }

    // Check if the parsed JSON is an array
    if (!json_object_is_type(parsed_json, json_type_array))
    {
        fprintf(stderr, "JSON content is not an array\n");
        json_object_put(parsed_json);
        return;
    }

    // Get the array length and process it in case the array has len of 64 and the first element is an int
    size_t array_len = json_object_array_length(parsed_json);
    if (array_len == 64)
    {
        // Check if the first element is an integer
        struct json_object *first_elem = json_object_array_get_idx(parsed_json, 0);
        if (json_object_is_type(first_elem, json_type_int))
        {
            fprintf(output_file, "File: %s\nContent: ", file_path);
            for (size_t i = 0; i < array_len; i++)
            {
                struct json_object *elem = json_object_array_get_idx(parsed_json, i);
                if (json_object_is_type(elem, json_type_int))
                {
                    fprintf(output_file, "%d", json_object_get_int(elem));
                    if (i < array_len - 1)
                    {
                        fprintf(output_file, ",");
                    }
                }
            }
            fprintf(output_file, "\n");
        }
        else
        {
            fprintf(stderr, "First element is not an integer: %s\n", file_path);
        }
    }

    json_object_put(parsed_json);
}

void traverse_directory(const char *directory_path, FILE *output_file)
{
    DIR *dir;

    struct dirent *entry;
    struct stat entry_info;
    char path[1024];

    if ((dir = opendir(directory_path)) == NULL)
    {
        perror("opendir() error");
        return;
    }

    // Debug output (uncomment to see a lot of text in the output :D)
    // printf("Traversing directory: %s\n", directory_path);

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", directory_path, entry->d_name);

        if (stat(path, &entry_info) == 0)
        {
            // if it's a directory - continue traversing
            if (S_ISDIR(entry_info.st_mode))
            {
                traverse_directory(path, output_file);
            }
            // if it's a file - process it
            else if (S_ISREG(entry_info.st_mode))
            {
                if (strstr(entry->d_name, ".json") != NULL)
                {
                    // Process the JSON file
                    printf("Processing JSON file: %s\n", path);
                    process_json_file(path, output_file);
                }
            }
        }
        else
        {
            perror("stat() error");
        }
    }

    closedir(dir);
}

int main()
{
    FILE *output_file = fopen("output.txt", "w");
    if (!output_file)
    {
        perror("fopen() error");
        return 1;
    }

    // starting from the root directory
    traverse_directory("/", output_file);

    fclose(output_file);
    return 0;
}