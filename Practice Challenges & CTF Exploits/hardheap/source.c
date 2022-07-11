/*
    Double-Free exploit, with an added fasttop bypass
    To ignore the `malloc(): memory corruption (fast)` check, this binary fakes the metadata
*/

#include <stdio.h>
#include <stdlib.h>


// Ignore
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	gid_t gid = getegid();
	setresgid(gid, gid, gid);
}


//Actual Code
char fakemetadata[0x10] = "\x30\0\0\0\0\0\0\0"; // so we can ignore the "wrong size" error
char admin[0x10] = "Nuh-huh\0";

// List of users to keep track of
char *users[15];
int userCount = 0;

void createUser() {
    char *name = malloc(0x20);
    users[userCount] = name;

    printf("%s", "Name: ");
    read(0, name, 0x20);

    printf("User Index: %d\nName: %s\nLocation: %p\n", userCount, users[userCount], users[userCount]);
    userCount++;
}

void deleteUser() {
    printf("Index: ");

    char input[2];
    read(0, input, sizeof(input));
    int choice = atoi(input);


    char *name = users[choice];
    printf("User %d:\n\tName: %s\n", choice, name, name);

    // Check user actually exists before freeing
    if(choice < 0 || choice >= userCount) {
        puts("Invalid Index!");
        return;
    }
    else {
        free(name);
        puts("User freed!");
    }
}

void complete_level() {
    if(strcmp(admin, "admin\n")) {
        puts("Level Complete!");
        return;
    }
}

void main_loop() {
    while(1) {
        printf(">> ");

        char input[2];
        read(0, input, sizeof(input));
        int choice = atoi(input);

        switch (choice)
        {
            case 1:
                createUser();
                break;
            case 2:
                deleteUser();
                break;
            case 3:
                complete_level();
            default:
                break;
        }
    }
}

int main() {
    setup();
    printf("Fake Metadata: %p\n", fakemetadata);
    main_loop();

    return 0;
}