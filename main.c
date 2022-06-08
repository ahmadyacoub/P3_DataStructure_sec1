// Ahmad Jaghama 1202450 sec #1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 256

int TABLE_SIZE1 = 7;
int linearCollisions = 0;
int TABLE_SIZE2 = 7;
int doubleCollisions = 0;
int loadFactor1 = 0;
int loadFactor2 = 0;


// linearHash size = first prime > 2* size of elements
int IsPrime(int n) {
    int i;
    for (i = 2; i <= n / i; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int FirstPrime(int n) {
    while (!IsPrime(n))
        n++;
    return n;
}

typedef struct {
    char courseName[MAX_NAME];
    int hours;
    char CourseCode[200];
    char depart[200];
    char topics[1000];


} Course;

Course *linearAfterRehash;
Course *DoubleAfterRehash;

Course *searchDouble(Course *doubleHash[], char *name);

void rehashLinear(Course hash_Table[]);

void display(Course *doubleHash[]);

Course *linearRehash(Course linearMainTable[]);

Course *
insertDouble(Course *doubleHashTable[], char *courseName, char *CourseCode, char *depart, char *topics, int hours);


Course *hash_Table_LOOKUP(Course *hash_Table[], char *name);

void init_Hash_Table(Course *hash_Table[]);

int number = 0;
int loadCoursesLimit = 0;


unsigned int linearHash(char *name) {
    int hashValue = 0;
    while (*name != '\0') {
        hashValue = (hashValue << 5) + *name++;
    }
    if (hashValue < -1)
        hashValue *= -1;
    return (hashValue % TABLE_SIZE1);

}

unsigned int doubleHash(char *name) {
    int hashValue = 0;
    while (*name != '\0') {
        hashValue = (hashValue << 5) +
                    *name++;
    }
    if (hashValue < -1)
        hashValue *= -1;
    return (hashValue % TABLE_SIZE2);

}

void print_Hash_Table(Course *hash_Table[]) {
    printf("start Linear Table \n");
    for (int i = 0; i < TABLE_SIZE1; ++i) {
        if (hash_Table[i] == NULL)
            printf("\t\tIndex %d  : --------\n", i);

        else
            printf("\t\tIndex %d  :  %s\n", i, hash_Table[i]->courseName);
    }
    printf("End Linear Table  \n");
}


Course *hash_Table_Insert(
        Course *hash_Table[], char *courseName, char *CourseCode, char *depart, char *topics, int hours) {

//    if (hash_Table_LOOKUP(hash_Table, courseName) != NULL) {
//        return hash_Table;
//    }

    int hash_Index = linearHash(courseName);

    for (int i = 0; i < TABLE_SIZE1; i++) {

        int try = (i + hash_Index) % TABLE_SIZE1;
        if (try < 0)
            try *= -1;
        if (hash_Table[try] == NULL) {
            hash_Table[try] = malloc(sizeof(Course));
            strcpy(hash_Table[try]->courseName, courseName);
            strcpy(hash_Table[try]->CourseCode, CourseCode);
            strcpy(hash_Table[try]->depart, depart);
            strcpy(hash_Table[try]->topics, topics);
            hash_Table[try]->hours = hours;

            loadFactor1++;
            if (i > 0)
                linearCollisions++;

            if (loadFactor1 > TABLE_SIZE1 / 2) {
                loadFactor1 = 0;
                linearCollisions = 0;
                hash_Table = linearRehash(&hash_Table);
            }

            return hash_Table;
        }
    }

    return hash_Table;
}

Course *hash_Table_InsertFromConsole(Course *hash_Table[], char *name, char *code, int h, char *depart, char *topics) {

    int hash_Index = linearHash(name);
    int try;

    for (int i = 0; i < TABLE_SIZE1; i++) {

        try = (i + hash_Index) % TABLE_SIZE1;
        if (hash_Table[try] == NULL) {

            hash_Table[try] = malloc(sizeof(Course));
            strcpy(hash_Table[try]->courseName, name);
            strcpy(hash_Table[try]->CourseCode, code);
            strcpy(hash_Table[try]->depart, depart);
            strcpy(hash_Table[try]->topics, topics);
            hash_Table[try]->hours = h;
            loadFactor1++;
            if (i > 0)
                linearCollisions++;

            if (loadFactor1 > TABLE_SIZE1 / 2) {
                loadFactor1 = 0;
                linearCollisions = 0;
                hash_Table = linearRehash(hash_Table);
            }

            return hash_Table;
        }
    }

}


Course *linearRehash(Course hash_Table[]) {

    int temp = TABLE_SIZE1;
    TABLE_SIZE1 = FirstPrime(2 * TABLE_SIZE1);
    linearAfterRehash = malloc(TABLE_SIZE1 * sizeof(Course));
    init_Hash_Table(linearAfterRehash);

    int i = 0;
    while (i < temp) {
        if (strcmp(hash_Table[i].courseName, "\0") != 0) {

            linearAfterRehash = hash_Table_Insert(linearAfterRehash, hash_Table[i].courseName,
                                                  hash_Table[i].CourseCode, hash_Table[i].depart,
                                                  hash_Table[i].topics, hash_Table[i].hours);
        }
        i++;
    }
    return linearAfterRehash;
}

Course *doubleRehash(Course doubleHashTable[]) {

    int temp = TABLE_SIZE2;
    TABLE_SIZE2 = FirstPrime(2 * TABLE_SIZE2);
    DoubleAfterRehash = malloc(TABLE_SIZE2 * sizeof(Course));
    init_Hash_Table(DoubleAfterRehash);
    int i = 0;
    while (i < temp) {
        if (strcmp(doubleHashTable[i].courseName, "\0") != 0) {

            DoubleAfterRehash = insertDouble(linearAfterRehash, doubleHashTable[i].courseName,
                                             doubleHashTable[i].CourseCode, doubleHashTable[i].depart,
                                             doubleHashTable[i].topics, doubleHashTable[i].hours);
        }
        i++;
    }
    return DoubleAfterRehash;
}

void init_Hash_Table(Course *hash_Table[]) { //makes the table empty

    for (int i = 0; i < TABLE_SIZE1; ++i) {
        hash_Table[i] = NULL;
    }// TABLE IS EMPTY
}


Course *hash_Table_Delete(Course *hash_Table[], char *name) {


    if (hash_Table_LOOKUP(hash_Table, name) == NULL)
        return NULL;
    int index = linearHash(name);
    for (int i = 0; i < TABLE_SIZE1; ++i) {
        int try = (i + index) % TABLE_SIZE1;
        if (try < 0)
            try *= -1;

        if (hash_Table[try] != NULL && strcmp(hash_Table[try], name) == 0) {
            printf("herre");
            Course *temp = hash_Table[try];
            hash_Table[try] = NULL;
            loadFactor1--;
            return temp;
        }
    }


    return NULL;
}

Course *double_Table_Delete(Course *hash_Table[], char *name) {


    int index, i, h1, h2;
    h1 = doubleHash(name);
    h2 = h1 - FirstPrime(TABLE_SIZE2 / 2);
    if (h2 < 0)
        h2 *= -1;
    if (searchDouble(hash_Table, name) == NULL)
        return NULL;
    index = (h1 + i * h2) % TABLE_SIZE2;
    if (index < 0)
        index *= -1;
    for (int i = 0; i < TABLE_SIZE2; ++i) {
        int try = (i + index) % TABLE_SIZE2;

        if (hash_Table[try] != NULL && strcmp(hash_Table[try], name) == 0) {

            Course *temp = hash_Table[try];
            hash_Table[try] = NULL;
            loadFactor2--;
            return temp;
        }
    }


    return NULL;
}


Course *hash_Table_LOOKUP(Course *hash_Table[], char *name) {
    int hash_Index = linearHash(name);

    for (int i = 0; i < TABLE_SIZE1; ++i) {
        int try = (i + hash_Index) % TABLE_SIZE1;
        if (try < 0)
            try *= -1;

        if (hash_Table[try] != NULL && strcmp(name, hash_Table[try]->courseName) == 0) {

            printf("\nits Code is %s name %s hours %d depart %s", hash_Table[try]->CourseCode,

                   hash_Table[try]->courseName, hash_Table[try]->hours, hash_Table[try]->depart);

            return hash_Table[try];
        }
    }
    printf("\nTHE COURSE ISNT IN THE LINEAR TABLE");
    return NULL;
}

Course *search(Course *hash_Table[], char *key) {
    //get the linearHash
    int hashIndex = linearHash(key);

    //move in array until an empty
    while (hash_Table[hashIndex] != NULL) {

        if (strcmp(hash_Table[hashIndex]->courseName, key) == 0)
            return hash_Table[hashIndex];
        //go to next cell
        ++hashIndex;
        //wrap around the table
        hashIndex %= TABLE_SIZE1;
    }

    return NULL;
}
//---------------------------------------------------------------------


Course *insertDoubleFromConsole(Course *doubleHashTable[], char *name, char *code, int h, char *depart, char *topics) {
    int index, i, h1, h2;

    h1 = doubleHash(name);
    h2 = h1 - FirstPrime(TABLE_SIZE2 / 2);
    if (h2 < 0)
        h2 *= -1;
    for (i = 0; i < TABLE_SIZE2; i++) {
        index = (h1 + (i * h2)) % TABLE_SIZE2;
        if (index < 0)
            index *= -1;
        if (doubleHashTable[index] == NULL) {
            doubleHashTable[index] = malloc(sizeof(Course));
            strcpy(doubleHashTable[index]->courseName, name);
            strcpy(doubleHashTable[index]->CourseCode, code);
            strcpy(doubleHashTable[index]->depart, depart);
            strcpy(doubleHashTable[index]->topics, topics);
            doubleHashTable[index]->hours = h;
            loadFactor2++;
            if (i > 0)
                doubleCollisions++;

            if (loadFactor2 > TABLE_SIZE2 / 2) {
                loadFactor2 = 0;
                doubleCollisions = 0;
                doubleHashTable = doubleRehash(&doubleHashTable);
            }

            return doubleHashTable;
        }
    }


}


Course *
insertDouble(Course *doubleHashTable[], char *courseName, char *CourseCode, char *depart, char *topics, int hours) {
    int try, i, h1, h2;

    h1 = doubleHash(courseName);
    h2 = h1 - FirstPrime(TABLE_SIZE2 / 2);
    if (h2 < 0)
        h2 *= -1;


    for (i = 0; i < TABLE_SIZE2; i++) {
        try = (h1 + (i * h2)) % TABLE_SIZE2;

        if (try < 0)
            try *= -1;
        if (doubleHashTable[try] == NULL) {
            doubleHashTable[try] = malloc(sizeof(Course));
            strcpy(doubleHashTable[try]->courseName, courseName);
            strcpy(doubleHashTable[try]->CourseCode, CourseCode);
            strcpy(doubleHashTable[try]->depart, depart);
            strcpy(doubleHashTable[try]->topics, topics);
            doubleHashTable[try]->hours = hours;
            loadFactor2++;
            if (i > 0)
                doubleCollisions++;

            if (loadFactor2 > TABLE_SIZE2 / 2) {
                loadFactor2 = 0;
                doubleCollisions = 0;
                doubleHashTable = doubleRehash(&doubleHashTable);
            }

            return doubleHashTable;
        }
    }
    if (i == TABLE_SIZE2)
        printf("\n element cannot be inserted its code %s and name\n", CourseCode, courseName);
    return doubleHashTable;
}

Course *searchDouble(Course *doubleHashTable[], char *name) {
    int index, i, h1, h2;
    h1 = doubleHash(name);
    h2 = h1 - FirstPrime(TABLE_SIZE2 / 2);
    if (h2 < 0)
        h2 *= -1;

    for (i = 0; i < TABLE_SIZE2; i++) {
        index = (h1 + i * h2) % TABLE_SIZE2;
        if (index < 0)
            index *= -1;

        if (doubleHashTable[index] != NULL && strcmp(name, doubleHashTable[index]->courseName) == 0) {

            printf("\nits found at index %d Code is %s name %s hours %d depart %s", index,
                   doubleHashTable[index]->CourseCode, doubleHashTable[index]->courseName,
                   doubleHashTable[index]->hours,
                   doubleHashTable[index]->depart);
            return &doubleHashTable[index];
        }
    }
    printf("\nTHE COURSE THAT %s isnt in Double Table", name);
    return NULL;
}

void display(Course *doubleHash[]) {
    printf("start Double Table \n");
    for (int i = 0; i < TABLE_SIZE2; ++i) {
        if (doubleHash[i] == NULL)
            printf("\t\tIndex %d  : --------\n", i);

        else
            printf("\t\tIndex %d  :  %s \n", i, doubleHash[i]->courseName);
    }
    printf("End Double Table  \n");
}

//-------------------------------- ------------------------------------
//void rehashLinear(Course *hash_Table[]) {
//    int tempSize = TABLE_SIZE1;
//    TABLE_SIZE1 = FirstPrime(2 * TABLE_SIZE1);
//    Course *temp = (Course *) malloc(sizeof(Course) * TABLE_SIZE1);
//
//    for (int i = 0; i < tempSize; ++i) {
//        if (hash_Table[i] != NULL) {
//            hash_Table_Insert(temp, hash_Table[i]);
//
//        }
//    }
//
//    free(hash_Table);
//    hash_Table = temp;
//
//}
char *scanString() {
    // this function is used to scan a char from console better
    // than scanf that is not efficient
    char *temp[200];
    fgets(temp, 200, stdin);

    int len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n')
        temp[len - 1] = '\0';
    return temp;
}

void traversePrint(Course *hashTABLE[], Course *doubleHash[], FILE *fp) {
    // IN CASE 10 THAT NEEDS TO PRINT A FILE I FACED A PROBLEM ,I SOLVED IT BY USING FUNCTIONS

    fprintf(fp, "start Linear Table \n");
    for (int i = 0; i < TABLE_SIZE1; ++i) {
        if (hashTABLE[i] == NULL)
            fprintf(fp, "\t\tIndex %d  : --------\n", i);

        else
            fprintf(fp, "\t\tIndex %d  :  %s\n", i, hashTABLE[i]->courseName);
    }
    fprintf(fp, "End Linear Table  \n");
    fprintf(fp, "start Double Table \n");
    for (int i = 0; i < TABLE_SIZE2; ++i) {
        if (doubleHash[i] == NULL)
            fprintf(fp, "\t\tIndex %d  : --------\n", i);

        else
            fprintf(fp, "\t\tIndex %d  :  %s\n", i, doubleHash[i]->courseName);
    }
    fprintf(fp, "End Double Table  \n");
}

void printToFile(Course *hashTABLE[], Course *doubleHash[]) {
    FILE *fp; // DECLARE A FILE TO ADD THE OUTPUT IN CASE 10
    fp = fopen("saved_courses.txt", "w");

    traversePrint(hashTABLE, doubleHash, fp); // TRAVERSING THE TREE AND PRINT THE DATA

    fclose(fp); // CLOSE THE FILE
}

int main() {
    Course hash_Table[7];
    init_Hash_Table(hash_Table);
    Course doubleHash[7];
    init_Hash_Table(doubleHash);

    char line[255];
    int len;
    char array[1000][255];
    int a = 0;

    FILE *coursesFile = fopen("offered_courses.txt", "r");

    if (loadCoursesLimit == 0) {
        while (!feof(coursesFile)) { // READING THE COURSES FILE
            fgets(line, 255, coursesFile);
            len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') // TO CHECK THE TRAILING LINE FROM FGETS
                line[len - 1] = '\0';
            strcpy(array[a], line);
            a++;
        }

        fclose(coursesFile); // closing the file
        for (int i = 0; i < a; ++i) {
            char *tok = NULL; // to poin into the separated string
            char topics[1000];

            tok = strtok(array[i], ":");

            Course *courseTemp = (Course *) malloc(sizeof(Course));
            int c = 1;

            while (tok != NULL && c < 7) {

                switch (c) {

                    case 1:
                        strcpy(courseTemp->courseName, tok);

                        tok = strtok(NULL, "#");

                        break;
                    case 2:
                        courseTemp->hours = atoi(tok);


                        tok = strtok(NULL, "#");

                        break;
                    case 3:
                        strcpy(courseTemp->CourseCode, tok);
                        tok = strtok(NULL, "/");

                        break;

                    case 4:
                        strcpy(courseTemp->depart, tok);
                        break;
                    case 6:
                        tok = strtok(NULL, ",");
                        while (tok != NULL) {

                            strcat(topics, tok);
                            tok = strtok(NULL, ",");
                        }
                        strcpy(courseTemp->topics, topics);


                        free(tok);
                        break;

                    default:

                        break;
                }
                c++;
            }
            linearAfterRehash = hash_Table_Insert(hash_Table, courseTemp->courseName, courseTemp->CourseCode,
                                                  courseTemp->depart, courseTemp->topics, courseTemp->hours);
            DoubleAfterRehash = insertDouble(doubleHash, courseTemp->courseName, courseTemp->CourseCode,
                                             courseTemp->depart, courseTemp->topics, courseTemp->hours);

            memset(topics, 0, strlen(topics)); // to clear the string



        }
        loadCoursesLimit++; // to avoid load the file again

    } else {
        printf("\t\t\t>>>>The file is already LOADED  <<<< :)");

    }

//    Course ahmad = {"Introduction to French", 3, "COMP2", "4", "323sacac"};
//    Course a2hmad = {"ADDED FROM INSERT NOT FILE", 3, "COMP2", "4", "323sacac"};
//    Course a4hmad = {"ADDED FROM INSERT NOT FILE", 3, "COMP2", "4", "323sacac"};
//    Course a3hmad = {"Introduction to", 3, "COMP2", "4", "323sacac"};
//    Course a11hmad = {"HAHAHHA FROM INSERT NOT FILE", 3, "COMP2", "depart", "323sacac"};
//    Course a22hmad = {"HHEHHEHE FROM INSERT NOT FILE", 3, "COMP2", "4", "323sacac"};
//    Course a33hmad = {"HOHOHO to ", 3, "COMP2", "4", "323sacac"};
//    Course a223hmad = {"H2123OHOHO to", 3, "COMP2", "4", "323sacac"};
//    hash_Table_Insert(hash_Table, &a11hmad);
//    hash_Table_Insert(hash_Table, &a22hmad);
//    hash_Table_Insert(hash_Table, &a33hmad);
//    hash_Table_Insert(hash_Table, &a3hmad);
//    hash_Table_Insert(hash_Table, &ahmad);
//    hash_Table_Insert(hash_Table, &a2hmad);
//    hash_Table_Insert(hash_Table, &a4hmad);
//    hash_Table_Insert(hash_Table, &a3hmad);
//    hash_Table_Insert(hash_Table, &a223hmad);
//    hash_Table_Insert(hash_Table, &a3hmad);
//    insertDouble(doubleHash, &ahmad);
//    insertDouble(doubleHash, &a2hmad);
//    insertDouble(doubleHash, &a3hmad);
//1
//    hash_Table_Delete(hash_Table, "to programming");
//    hash_Table_Delete(hash_Table, "Data structures");
//
//
//    hash_Table_Delete(hash_Table, "Introduction to French");
//    hash_Table_Delete(hash_Table, "Introduction to ");
//    printf("\n%s\n", hash_Table_LOOKUP(hash_Table, "Introduction to ")->courseName);
//    hash_Table_Delete(hash_Table, "to programming");
//    hash_Table_Delete(hash_Table, "ADDED FROM INSERT NOT FILE");

    char switchLine[200];
    int num, x;
    char *code;
    char codes[200];
    char depart[200];
    char courseName[200];
    char topics[200];
    int hours;
    while (1) { // a while loop for program menu
        printf("\n------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t1-Print Tables\n");
        printf("\t\t\t2-Print Table Size & loadFactor for each \n");
        printf("\t\t\t3-Print Used Functions  \n");
        printf("\t\t\t4-Insert a new record to tables \n");
        printf("\t\t\t5-Search for a specific word \n");
        printf("\t\t\t6-Delete a specific record \n");
        printf("\t\t\t7-Compare between the two methods in terms of number of collisions occurred \n");
        printf("\t\t\t8- Save all words in a file\n");
        printf("\t\t\t-1 - Exit :) \n");
        printf(" ENTER YOUR CHOICE :\t");

        fgets(switchLine, 10, stdin);
        int len = strlen(switchLine);
        if (len > 0 && switchLine[len - 1] == '\n')
            switchLine[len - 1] = '\0';
        num = atoi(switchLine);
        printf("\n------------------------------------------------------------------------------------------------\n");
        switch (num) {


            case 1:
                display(doubleHash);
                print_Hash_Table(hash_Table);

                break;
            case 2:
                printf("\n\t\t\tFor linear Table the size is %d  and the loadFactor %f ", TABLE_SIZE1,
                       (loadFactor1 * 1.0) / TABLE_SIZE1);
                printf("\n\t\t\tFor Double Table the size is %d  and the loadFactor %f ", TABLE_SIZE2,
                       (loadFactor2 * 1.0) / TABLE_SIZE2);
                break;


            case 3: // Course Code? will it be updated ? and topics?
                printf("\n\t\tFor linear linearHash  : (i + hash_Index) % TABLE_SIZE ");
                printf("\n\t\tFor Double Hash  :  (h1 + i * h2) % TABLE_SIZE");
                printf("\n\t\tWHERE AS  h1 = the hash of (course->courseName)\n"
                       "       \t\t\t  h2 =h1 - FirstPrime(TABLE_SIZE2 / 2) \n");
                break;
            case 4:
                fflush(stdin); // to clear console
                printf("\n \t\t\tEnter the course code to be ADDED :>\t");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n') // remove trailing line
                    code[len - 1] = '\0';
                strcpy(codes, code); //  i used pointer func
                fflush(stdin);
                printf("\n \t\t\tEnter the course name to be ADDED :>\t");

                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';
                strcpy(courseName, code); //  i used pointer func

                fflush(stdin); // clear the console
                printf("\n \t\t\t Enter the course department to be ADDED :>\t");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';
                strcpy(depart, code); //  i used pointer func
                fflush(stdin);
                printf("\n \t\t\t Enter the course hours to be ADDED :>\t");
                scanf("%d", &hours);
                fflush(stdin);
                printf("\n \t\t\t Enter the course topics to be ADDED :>\t");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';
                strcpy(topics, code); //  i used pointer func

                linearAfterRehash = hash_Table_InsertFromConsole(hash_Table, courseName, codes, hours, depart, topics);

                DoubleAfterRehash = insertDoubleFromConsole(doubleHash, courseName, codes, hours, depart, topics);


                break;

            case 5:

                printf("\n\tEnter the course Name : ");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n')
                    code[len - 1] = '\0';

                printf("\n1- Linear Table ");
                printf("\n2- Double Table ");
                fgets(switchLine, 10, stdin);
                int len = strlen(switchLine);
                if (len > 0 && switchLine[len - 1] == '\n')
                    switchLine[len - 1] = '\0';
                x = atoi(switchLine);
                if (x == 1) {
                    hash_Table_LOOKUP(hash_Table, code);
                    break;

                } else if (x == 2) {
                     searchDouble(doubleHash, code);
                    break;
                }

            case 6:
                fflush(stdin); // to clear console
                printf("\n \t\t\tEnter the course code to be Deleted :>\t");
                code = scanString();
                len = strlen(code);
                if (len > 0 && code[len - 1] == '\n') // remove trailing line
                    code[len - 1] = '\0';
                strcpy(codes, code); //  i used pointer func
                hash_Table_Delete(hash_Table, codes);
                double_Table_Delete(doubleHash, codes);


                break;
            case 7:
                printf("\n\t\t\t\tLinear collisions %d", linearCollisions);
                printf("\n\t\t\t\tDouble collisions %d", doubleCollisions);
                break;
            case 8:
                printToFile(hash_Table, doubleHash);
                printf("\nTHE TABLES ARE PRINTED");
                break;
            case -1:
                printf("\t\t\t\t\t<    <    <    F  I  N  I  S  H  E  D    >    >    >    ");
                printf("\n------------------------------------------------------------------------------------------------\n");
                exit(0);
            default:
                printf("\n Wrong Input Re Enter \n ");
                break;


        }


    }

    return 0;
}
