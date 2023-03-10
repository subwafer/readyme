#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

bool DEBUG_MODE = false;

struct project {
    // TODO: Convert to pointers when relevant. From stack to heap.
    char README_FILE_PATH[300];
    char project_name[100];
    char summary[1024];

    int multiple_authors;
    char author_name[100];
    char author_email[200];
    char author_role[100];
    char authors[100][400];
};

void create_readme_file(struct project *p);
void strip_newline(int length, char arr[]);
void prompt_for_project_name(struct project *p);
void prompt_for_summary(struct project *p);
void prompt_for_author_temp(struct project *p);
void generate_c_boilerplate();
void generate_build_file();

// TODO: Refactor
// TODO: create a banner in c code with info from user
// TODO: Make this working in live environment (exe move and use)
// TODO: Create dirs
// TODO: 2 modes: "Simple" for quick project. Or scaffold for my setup.

void handle_cli_args(int argc, char **argv);

void handle_cli_args(int argc, char **argv) {
    (void) argc;
    (void) argv;

    /*
    ** config command
    ** Allow user to create templates.
    ** example: rbk config --templates
    **
    ** example: rbk create . --template=cgui --sc=git
    **
     */

    printf("handle_cli_args not implemented!");
}

int main(int argc, char *argv[]) {
    struct project p;

    // NOTE: Look into getopt GNU for posix func for reading cli args
    // OR you can just write your own. argv is a pointer to pointer or a pointer to a char array
    // handle_cli_args(argc, argv);
    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--debug") != 0) {
                DEBUG_MODE = true;
            }
        }
    }

    if (DEBUG_MODE == false) {
        strcpy(p.README_FILE_PATH, "example/test_README.md");
        printf(">>> DEBUG MODE ACTIVE. Creating file: %s<<<<\n", p.README_FILE_PATH);
    } else {
        strcpy(p.README_FILE_PATH, "example/README.md");
    }

    // TODO: Switch statement per args

    prompt_for_project_name(&p);
    prompt_for_summary(&p);
    prompt_for_author_temp(&p);
    create_readme_file(&p);

    generate_c_boilerplate();
    generate_build_file();

    return 0;
}

void create_readme_file(struct project *p) {
    FILE *file;

    file = fopen(p->README_FILE_PATH, "w");
    if (file == NULL) {
        printf("ERROR: Unable to create file\n");
        exit(1);
    }

    // TODO: Separate this out to make it easier to read
    fprintf(file, "# %s\n\n## Summary\n\n%s\n\n## Author(s)\n\n%s | %s | %s\n\n",
        p->project_name, p->summary, p->author_name, p->author_email, p->author_role);

    printf("Successfully Generated README file\n");
}

void strip_newline(int length, char arr[]) {
    if (arr[length - 1] == '\n') {
        arr[length - 1] = '\0';
    }
}

void prompt_for_project_name(struct project *p) {
    printf("Project name > ");
    fgets(p->project_name, 100, stdin);

    strip_newline(strlen(p->project_name), p->project_name);
}

void prompt_for_summary(struct project *p) {
    printf("Summary > ");
    fgets(p->summary, 1024, stdin);

    strip_newline(strlen(p->summary), p->summary);
}

void prompt_for_author_temp(struct project *p) {
    printf("Author name > ");
    fgets(p->author_name, 100, stdin);

    strip_newline(strlen(p->author_name), p->author_name);

    printf("Author email > ");
    fgets(p->author_email, 200, stdin);

    strip_newline(strlen(p->author_email), p->author_email);

    printf("Author role > ");
    fgets(p->author_role, 100, stdin);

    strip_newline(strlen(p->author_role), p->author_role);
}

void generate_c_boilerplate() {
    // TODO: Create the 'example' directory if it is not found
    FILE *file;

    file = fopen("example/test_main.c", "w");
    if (file == NULL) {
        printf("ERROR: Unable to create c boilerplate file\n");
        exit(1);
    }

    // Creates a basic hello, world c program
    fprintf(file, "/*\n");
    fprintf(file, "\tProject generated by readyme cli\n");
    fprintf(file, "*/\n\n");
    fprintf(file, "#include <stdio.h>\n\n");
    fprintf(file, "int main(int argc, char **argv) {\n");
    fprintf(file, "\t(void) argc;\n");
    fprintf(file, "\t(void) argv;\n\n");
    fprintf(file, "\tchar hello[] = \"Hello, World!\";\n");
    fprintf(file, "\tprintf(\"%%s\\n\", hello);\n\n");
    fprintf(file, "\treturn 0;\n");
    fprintf(file, "}\n");

    printf("Successfully Generated C boilerplate file\n");
}

void generate_build_file() {
    FILE *file;

    char file_path[] = "example/test_build.sh";

    file = fopen(file_path, "w");
    if (file == NULL) {
        printf("ERROR: Unable to create builder file\n");
        exit(1);
    }

    // NOTE: This assumes A LOT of the persons OS + environment.
    // Should extend later to handle windows, other things.
    // Maybe allow the user to customize this?
    fprintf(file, "# Generated by readyme cli\n\n");
    fprintf(file, "#!/bin/sh\n\n");
    fprintf(file, "set -xe\n\n");
    fprintf(file, "# CC=clang\n\n");
    fprintf(file, "# CFLAGS=\"-Wall -Wextra -std=c11 -pedantic -ggdb -fsanitize=memory\"\n\n");
    fprintf(file, "CC=gcc\n");
    fprintf(file, "CFLAGS=\"-Wall -Wextra -std=c11 -pedantic -ggdb\"\n\n");
    fprintf(file, "$CC $CFLAGS -o main main.c\n\n");

    // -> -rwxr-xr-x
    chmod(file_path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    printf("Successfully Generated builder file\n");
}
