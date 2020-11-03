#include "../hdr/includes.h"

void read_from_file(char *file_name)
{
  FILE *dataset_x;
  char line[1000];
  dataset_x = fopen(file_name, "r");
  if (dataset_x == NULL)
  {
    perror("Unable to open dataset_x file!");
    exit(1);
  }
  else
  {
    while (fgets(line, 1000, dataset_x) != NULL)
    {
      puts(line);
    }
    fclose(dataset_x);
  }
}

void read_from_dir(char *dir_name, HashBucket **hash_table) {
  if ( hash_table == NULL ) {
    return;
  }
  DIR *sites_dir;
  struct dirent *sites_folder;

  // Open dir for site folders
  sites_dir = opendir(dir_name);

  if (sites_dir == NULL) {
    perror("Unable to read Dataset X base directory!");
    exit(1);
  }

  // For each site folder, open it and read the data.
  while( (sites_folder = readdir(sites_dir)) ) {
    if ( strcmp(sites_folder->d_name, ".") != 0 && strcmp(sites_folder->d_name, "..") != 0 ) {
      char *dir_path;
      DIR *sub_f;
      struct dirent *specs_folder;

      // Open the sub directory.
      dir_path = (char*)safe_malloc(strlen(dir_name) + strlen(sites_folder->d_name) + 1);
      sprintf(dir_path, "%s%s", dir_name, sites_folder->d_name);

      //printf("%s\n", dir_path);

      sub_f = opendir(dir_path);
      if ( sub_f == NULL ) {
        perror("Unable to read Dataset X sub directory!");
        exit(1);
      } else {
        while( (specs_folder = readdir(sub_f)) ) {
          if ( strcmp(specs_folder->d_name, ".") != 0 && strcmp(specs_folder->d_name, "..") != 0 ) {
            //printf("%s\n", specs_folder->d_name); // json files
            FILE *dataset_x;
            char line[1000];
            char *spec_path;

            // Open the spec file
            spec_path = (char*)safe_malloc(strlen(dir_path) + strlen(specs_folder->d_name) + 2);
            sprintf(spec_path, "%s/%s", dir_path, specs_folder->d_name);

            //printf("%s\n", spec_path);

            dataset_x = fopen(spec_path, "r");
            if ( dataset_x == NULL ) {
              perror("Unable to open Dataset X's file!");
              exit(1);
            } else {
              // Time to read the spec
              SpecInfo *new_spec = initSpecInfo("new", "spec");

              if ( fgets(line, 1000, dataset_x) != NULL ) {
                puts(line);
              }

              addToHashTable(hash_table, new_spec);

              fclose(dataset_x);
            }
            free(spec_path);
          }
        }
        closedir(sub_f);
      }
      free(dir_path);
    }
  }
  closedir(sites_dir);
}

int count_datafiles(char* path) {
  DIR *dir_ptr = NULL;
  struct dirent *direntp;
  char *npath;
  if ( !path ) {
    return 0;
  }
  if ( (dir_ptr = opendir(path)) == NULL ) {
    return 0;
  }

  int count = 0;
  while ( (direntp = readdir(dir_ptr)) ) {
    if ( strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0 ) {
      continue;
    }
    switch (direntp->d_type) {
      case DT_REG: {
        count++;
        break;
      }
      case DT_DIR: {
        npath = (char*)safe_malloc(strlen(path) + strlen(direntp->d_name) + 2);
        sprintf(npath, "%s%s", path, direntp->d_name);
        count += count_datafiles(npath);
        free(npath);
        break;
      }
    }
  }
  closedir(dir_ptr);
  return count;
}
