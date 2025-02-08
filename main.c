#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* long ass file paths : now covered !*/
#define BUFFER_SIZE 300000 

#ifndef BACKUP_INTERVAL 
#define BACKUP_INTERVAL 1800
#endif

char** read_backupfile(size_t *num_lines) {
  FILE *file;
  char buffer[BUFFER_SIZE];
  char **lines = NULL;
  size_t lines_count = 0;
  file = fopen("backupfile.txt", "r");
  if (file == NULL) {
    perror("backupfile.txt not found!");
    return NULL;
  }

  while(fgets(buffer, BUFFER_SIZE, file) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char *line = malloc(strlen(buffer) + 1);
    if(line == NULL) {
      perror("mem alloc failed!");
      fclose(file);
      return NULL;
    }
    strcpy(line, buffer);
    
    char **new_lines = realloc(lines, (lines_count + 1) * sizeof(char *));
    if (new_lines == NULL) {
      perror("mem alloc failed!");
      free(line);
      fclose(file);
      return NULL;
    }
    lines = new_lines;
    lines[lines_count] = line;
    lines_count++;
  }
  fclose(file);
  *num_lines =lines_count;
  return lines;
}

int main(int argc, char* argv[]) {
  while (1) {
        size_t num_lines = 0;
        char **file_content = read_backupfile(&num_lines);
        if (argc < 2) {
          printf("no backup path provided!\nusage: ./shittybkp /path/to/backup_directory\n");
          return 1;
        }
        printf("destination: %s\n", argv[1]);
        if (file_content != NULL) {
          for (size_t i = 0; i < num_lines; i++) {
            size_t command_length = snprintf(NULL, 0, "rsync -azP %s %s", file_content[i] , argv[1]) + 30000;
            char* _command = malloc(command_length);
            snprintf(_command, command_length, "rsync --delete -azP %s %s", file_content[i], argv[1]);
            printf("command: %s\n", _command);
            int result = system(_command);
            if (result != 0) {
                fprintf(stderr, "Error: rsync failed with status %d\n", result);
            } else {
                printf("Backup completed successfully.\n");
            }
            free(file_content[i]);
          }
          free(file_content);
        }
    sleep(BACKUP_INTERVAL);
  }
  return 0;
}


