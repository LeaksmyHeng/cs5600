#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1000
#define FILE_BUFFER 1024
#define SUCCESS 0
#define FAILURE 1

const char *commandarray[] = {"write", "get", "rm", "ls"};
// static directory to remote_file_path
char *prefix_remote_file = "remote_file_path/";


void copystring(char *destination, char* source) {
    /**
     * Copy string from source to destination.
     * This is used when getting the argument from argv.
     * */
    int i;
    for (i=0; source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    destination[i] = '\0';
}


int isincommandarray(char *searchstring) {
    /**
     * This function check if the command is in the command array.
     * For example, user's command can be WRITE, and it is in commandarray.
     * */
    int arraysize = sizeof(commandarray) / sizeof(commandarray[0]);
    int result;
    //printf("Searching for %s\n", searchstring);
    for (int i=0; i < arraysize; i++) {
        // compare string - case insensitive
        result = strcasecmp(searchstring, commandarray[i]);
        if (result == 0) {
            return i;
        }
    }
    // cannot return FAILURE like normal so we decide to return -1
    return -1;
}


int directoryorfileexist(const char *path) {
    /**
     * This function checks if a file or directory exist.
     * */
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}


int directoryexist(const char *path) {
    /**
     * This function checks if a directory exist.
     * */
    struct stat buffer;
    return (stat(path, &buffer) == 0 && S_ISDIR(buffer.st_mode));
}


int checkpath(const char *path) {
    /**
     * This function check if the path exist.
     * This is different from directory exist because it check
     * path by path rather than the entire path like if our path is folder1/folder2
     * Then it will first check folder1 then folder2 later, so we know exactly which folder is missing.
     * */
    char currentpath[MAX_PATH_LENGTH];
    const char *remainingpath = path;

    // split path on / and check each of them
    while ((remainingpath = strchr(remainingpath, '/')) != NULL) {
        strncpy(currentpath, path, remainingpath - path);
        currentpath[remainingpath - path] = '\0';
        // check if path exist
        if (!directoryorfileexist(currentpath)) {
            printf("Directory %s does not exist.\n", currentpath);
            return FAILURE;
        }
        ++remainingpath;
    }
    // check if file exist
    if (!directoryorfileexist(path)) {
        //printf("File %s not exist.\n", path);
        return FAILURE;
    }
    //printf("File %s exist.\n", path);
    return SUCCESS;
}


void removebackslash(char *string) {
    if (string != NULL && *string == '/') {
        size_t length = strlen(string + 1) + 1;
        memmove(string, string + 1, length);
    }
}


int createfile(char *path) {
    /**
     * Function to create file based on filename
     */
    // mode for creating file can be changed as need be
    int file = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (file == -1) {
        // if this fail, try another attempt without backslash
        removebackslash(path);
        int file = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (file == -1) {
            printf("Fail while creating file %s\n", path);
            return FAILURE;
        }
        else {
            return SUCCESS;
        }
    }
    close(file);
    return SUCCESS;
}


char *concatenatepath(const char* prefix_path, const char *path) {
    /**
     * Create a function to concatenate the file path.
     * */
    // allocate memory to combine the path
    // +2 because 1 for '/' and another for NULL terminator
    char *full_path = malloc(strlen(prefix_path) + strlen(path) + 1);
    if (full_path == NULL) {
        //printf("Fail while trying to allocate full path.\n");
        free(full_path);
        return NULL;
    }

    // copy prefix path and path
    strcpy(full_path, prefix_path);
    strcat(full_path, path);
    //printf("full path %s\n", full_path);
    return full_path;
}


int copy_file_content(int source_file, int destination_file) {
    /**
     * Function to copy content from one file to another.
     * */
    char buffer[FILE_BUFFER];
    int bytes_read;
    while ((bytes_read = read(source_file, buffer, sizeof(buffer))) > 0) {
        if (write(destination_file, buffer, bytes_read) != bytes_read) {
            printf("Error while writing into file.\n");
            close(source_file);
            close(destination_file);
            return FAILURE;
        }
    }
    return SUCCESS;
}


char *generate_version_file(char *filename, int version) {
    /**
     * Function to generate the name of the file with version if file already exist.
     * */
     // so that there is enough space for -vX
     char *versioned_filename = malloc(strlen(filename) + 5);
     if (versioned_filename == NULL) {
         return NULL;
     }

     // split string in '.', this is to get the .txt at the end of the file
     char *is_dot = strrchr(filename, '.');
     if (is_dot == NULL) {
         printf("This is not a file. This is a directory.\n");
         return NULL;
     }
     else {
         // copy the filename
         strncpy(versioned_filename, filename, is_dot - filename);
         versioned_filename[is_dot - filename] = '\0'; // end of character

         // append version
         snprintf(versioned_filename + (is_dot - filename), 5, "_v%d", version);

         // append file type
         strcat(versioned_filename, is_dot);
     }

     //printf("version file name: %s\n", versioned_filename);
     return versioned_filename;
}


void create_version_file(char *remote_filename, char *local_filename) {
    /**
     * If file already exist, this function will handle the versioning of the file.
     * */
     int version = 1;

     if (checkpath(remote_filename) == SUCCESS) {
         while (checkpath(generate_version_file(remote_filename, version)) == SUCCESS) {
             version++;
         }
         char *versioned_filename = generate_version_file(remote_filename, version);

         // open file for reading then open version file for writing to
         // reading the remote_file/file.txt
         int open_file = open(remote_filename, O_RDONLY);
         //printf("Open file name: %s\n", remote_filename);
         if (open_file < 0) {
             free(versioned_filename);
             printf("Failed to open file name: %s\n", remote_filename);
             return;
         }
         int open_versioned_file = open(versioned_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
         if (open_versioned_file < 0) {
             free(versioned_filename);
             close(open_file);
             close(open_versioned_file);
             printf("Failed to open file version: %s\n", versioned_filename);
             return;
         }

         // writing to versioned file content
         int result = copy_file_content(open_file, open_versioned_file);
         if (result == FAILURE) {
             printf("Fail to copy to file: %s\n", versioned_filename);
         }
         // writing from source file to remove file
         int open_local_file = open(local_filename, O_RDONLY);
         if (open_local_file < 0) {
             printf("Faied while trying to copy local file to remote file.\n");
             free(versioned_filename);
             return;
         }
         int open_remote_file = open(remote_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
         if (open_remote_file < 0) {
             free(versioned_filename);
             printf("Failed to open file name: %s\n", remote_filename);
             return;
         }
         // overwrite the remote file
         int result_final = copy_file_content(open_local_file, open_remote_file);
         if (result_final == FAILURE) {
             printf("Fail to copy to file\n");
         }
         free(versioned_filename);
         close(open_file);
         close(open_versioned_file);
     }
}


int get_file_information(int open_file, struct stat *stat_buf) {
    /**
     * Function to get file information.
     * */
    if (fstat(open_file, stat_buf) < 0) {
        printf("Failed to get source file information.\n");
        close(open_file);
        return FAILURE;
    }
    // to retrive information we can do
    // struct stat st => st.st_size, st.st_atime, st.st_mtime, st.st_mode
    return SUCCESS;
}


int create_file_structure(char* to_file, char* from_file) {
    // make a copy of destination file
    char *destination_copy = strdup(to_file);    // make a copy
    if (destination_copy == NULL) {
        printf("Make a copy of destination path failed.\n");
        return FAILURE;
    }

    char *token = strtok(destination_copy, "/");
    char *current_path = "";
    char *temp_path = NULL;

    // loop through each path in the root directory to see if it exist
    while (token != NULL) {
        char *temp_path_segment = malloc(strlen(current_path) + strlen(token) + 2);
        if (temp_path_segment == NULL) {
            printf("Fail while creating temp path.\n");
            free(destination_copy);
            return FAILURE;
        }

        // construct the temp_path_segment
        snprintf(temp_path_segment, strlen(current_path) + strlen(token) + 2, "%s%s%s", current_path,
                 (strcmp(current_path, "/") != 0) ? "/" : "", token);
        // constructing directory
        if (strchr(token, '.') == NULL) {
            if (directoryexist(temp_path_segment)) {
                //printf("Directory: exist %s\n", temp_path_segment);
                printf(" ");
            }
            else {
                // use temp_path_segment
                // printf("Directory: %s\n", temp_path_segment);
                // make directory if it does not exist
                removebackslash(temp_path_segment);
                if (mkdir(temp_path_segment, 0755) != 0) {
                    printf("Make directory error: %s\n", temp_path_segment);
                }
                //else {
                //    printf("Make directory success: %s\n", temp_path_segment);
                //}
            }
        }
        else {
            //printf("File: %s\n", temp_path_segment);
            createfile(temp_path_segment);

            // open source file in read only mode
            int opened_from_file = open(from_file, O_RDONLY);
            if (opened_from_file < 0) {
                printf("Failed while reading source file %s\n", from_file);
                return FAILURE;
            }
            // get from_file_information
            struct stat source_stat;
            if (get_file_information(opened_from_file, &source_stat) == FAILURE) {
                printf("Failed to get opened_from_file information.\n");
                close(opened_from_file);
                return FAILURE;
            }

            // open destination file as write mode
            int opened_to_file = open(temp_path_segment, O_WRONLY | O_CREAT | O_TRUNC, source_stat.st_mode);
            if (opened_to_file < 0) {
                printf("Failed to open destination file %s\n", temp_path_segment);
                close(opened_from_file);
                return FAILURE;
            }

            // copy from from_file to to_file
            int copy_content = copy_file_content(opened_from_file, opened_to_file);
            if (copy_content == FAILURE) {
                printf("Failed to copy from %s to %s.\n", from_file, temp_path_segment);
                return FAILURE;
            }
            // close file
            close(opened_from_file);
            close(opened_to_file);

        }
        // reassigned the temp_path to temp_path_segment
        current_path = temp_path_segment;
        token = strtok(NULL, "/");
        free(temp_path);
    }
    free(destination_copy);
    return SUCCESS;
}


void split_path(char *path, char **dir_path, char **file_path) {
    /**
     * Split path to get directory path and file_path.
     * linux.die.net/man/3/basename
     * */
     // make a copy of the path
     char *path_copy = strdup(path);
     *dir_path = strdup(path_copy);

    //basename and dirname is not working cause basename does
    // not give filename but rather the last folder of that file

     char *last_slash = strrchr(*dir_path, '/');
     if (last_slash != NULL) {
         *last_slash = '\0';
         *file_path = strdup(last_slash + 1);
     }
     else {
         *dir_path = NULL;
         *file_path = strdup(path_copy);
     }

     free(path_copy);
}


void split_string_on_dot(char *input, char **front, char **back) {
    /**
     * Splitting the string so we can get the front and back (file extention).
     * */
     char *temp = strdup(input);
     char *delimeter = ".";
     char *token = strtok(temp, delimeter);
     if (token != NULL) {
         *front = strdup(token);
         token = strtok(NULL, delimeter);
         if (token != NULL) {
             *back = strdup(token);
         }
         else {*back = NULL;}
     } else {
         *front = NULL;
         *back = NULL;
     }
     free(temp);
}


char* create_full_filename(char *filename, char *version_patter, int version_number, char *file_extention) {
    // 10 maximum number of digits but for this purpose I'll just put 2
    size_t length = strlen(filename) + strlen(version_patter) + 2 + strlen(file_extention) + 1;

    // allocate memory for full file name
    char *full_file_name = (char *)malloc(length);
    sprintf(full_file_name, "%s%s%d.%s", filename, version_patter, version_number, file_extention);
    return full_file_name;
}


void search_files(const char *path, char *file_name) {
    /**
     * Search files based on a specified path.
     * https://man7.org/linux/man-pages/man3/readdir.3.html
     * */
     DIR *dir = opendir(path);
     if (dir == NULL) {
         printf("Error while trying to open path %s\n", path);
         return;
     }
     struct dirent *entry;
     int matching_count = 0;

     char *front, *back;
     split_string_on_dot(file_name, &front, &back);

     while ((entry = readdir(dir)) != NULL) {
         // check if it is a regular file
         if (entry->d_type == DT_REG) {
             //if (strstr(entry->d_name, file_name) != NULL) {
             //    printf("Found match file: %s, %s, %d\n", file_name, entry->d_name, matching_count);
             //}
             matching_count ++;
         }
     }
     closedir(dir);

     for (int i = 0;  i < matching_count; i++) {
         char *file_name_version = file_name;
         if (i > 0) {
             file_name_version = create_full_filename(front, "_v", i, back);
         }
         //printf("%s file name\n", file_name_version);
         // open directory one more time
         DIR *dir = opendir(path);
         while ((entry = readdir(dir)) != NULL) {
             // check if it is a regular file
             if (entry->d_type == DT_REG) {
                 //printf("File name %s\n", entry->d_name, file_name_version);
                 if (strstr(entry->d_name, file_name_version) != NULL) {
                     printf("Found match file: %s\n", file_name_version);
                     // retrieve file information by:
                     // construct file path again
                     // open file first
                     // called get_file_information
                     char file_path_construction[MAX_PATH_LENGTH];
                     snprintf(file_path_construction, sizeof(file_path_construction), "%s/%s", path, file_name_version);

                     int opened_file = open(file_path_construction, O_RDONLY);
                     if (opened_file < 0) {
                         printf("Failed to open file name: %s\n", file_path_construction);
                        close(opened_file);
                     }
                     else {
                         struct stat source_stat;
                         if (get_file_information(opened_file, &source_stat) == FAILURE) {
                             printf("Failed to get opened_from_file information.\n");
                             close(opened_file);
                             return;
                         }
                         printf("File information:\nSize: %ld bytes\nLast modify time: %ld\nLast access time: %ld\n",
                                source_stat.st_size, source_stat.st_mtime, source_stat.st_atime);
                     }
                 }
             }
         }
         closedir(dir);
     }
}


int executing_write_command(char* source, char *destination) {
    /**
     * Function to execute WRITE command by copy from source (local_file)
     * to remote (remote_file specified under remote_path_file)
     * */
     // check if source file exist
    int source_exit = checkpath(source);
    if (source_exit == FAILURE) {
        printf("Local file %s does not exist.\n", source);
        return FAILURE;
    }

    // check if destination file exist
    // concatinate the destination file to the correct directory first
    char * destination_file = concatenatepath(prefix_remote_file, destination);
    int destination_exit = checkpath(destination_file);
    // if destination file already exist then create version file
    if (destination_exit == SUCCESS) {
        //printf("Destination file %s already exist. Make newer version.\n", destination_file);
        create_version_file(destination_file, source);
    }
    else {
        // destination file does not exist so we have to construct the path and
        // write the data to that path
        //printf("Destination file %s does not exist.\n", destination_file);

        int result = create_file_structure(destination_file, source);
        if (result == FAILURE) {
            printf("Failed while creating new file in remote.\n");
            return FAILURE;
        }
    }
    printf("Successfully execute WRITE command.\nCopy from: %s to %s\n\n", source, destination_file);
    return SUCCESS;
}


int executing_get_command(char *remote_file_path, char* local_file_path) {
    /**
     * Executing get command. This is to copy file form remote to local.
     * */
    char *full_remote_file_path = concatenatepath(prefix_remote_file, remote_file_path);
    // check if remote file exist
    int remote_file_exit = checkpath(full_remote_file_path);
    if (remote_file_exit == FAILURE) {
        printf("Remote file not found. Failed.\n");
        return FAILURE;
    }

    // check if local file exist
    // if it is, overwrite
    int local_file_exist = checkpath(local_file_path);
    if (local_file_exist == SUCCESS) {
        printf("Local file exist already, so will overwrite.\n");
    }
    int result = create_file_structure(local_file_path, full_remote_file_path);
    if (result == FAILURE) {
        printf("Failed while creating new file in remote.\n");
        return FAILURE;
    }
    printf("Successfully copying file from remote %s to local %s.\n\n", remote_file_path, local_file_path);
    return SUCCESS;
}


int executing_remove_command(const char *remote_file) {
    /**
    Function to remove file or folder
    */

    char *full_remote_file_path = concatenatepath(prefix_remote_file, remote_file);

    // check if directory
    if (strchr(full_remote_file_path, '.') == NULL) {
        if (rmdir(full_remote_file_path) == 0) {
            printf("Fail while trying to remove directory %s\n", full_remote_file_path);
            free(full_remote_file_path);
            return FAILURE;
        }
    }
    else {
        if (remove(full_remote_file_path) != 0) {
            printf("Fail while trying to remove file %s\n", full_remote_file_path);
            free(full_remote_file_path);
            return FAILURE;
        }
    }
    free(full_remote_file_path);
    printf("Successfully remove %s from remote\n\n", remote_file);
    return SUCCESS;
}


int executing_ls_command(const char *remote_file) {
    /**
     * Executing ls command that return the name of the file and all timestamps
     * when the versions were last written to..
     * */
     // check if path exist
    char * path = concatenatepath(prefix_remote_file, remote_file);
    int path_exit = checkpath(path);
    if (path_exit == FAILURE) {
        printf("Path %s does not exist.\n", remote_file);
        return FAILURE;
    }
    else {
        //printf("Path %s exist. Now split_path to get dir_path and file_path.\n", path);
        // split file_path to only get file name, not file extention
        char *dir_path, *file_name;
        split_path(path, &dir_path, &file_name);
        //printf("%s dir_path & %s file_path\n", dir_path, file_name);

        // navigate to directory path
        search_files(dir_path, file_name);

        free(dir_path);
        free(file_name);
    }
    printf("Successfully executing ls command.\n\n");
    return SUCCESS;
}
