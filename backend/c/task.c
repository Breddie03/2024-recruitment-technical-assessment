#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 50
#define MAX_CATEGORIES 10

struct File {
    int id;
    char name[MAX_STRING_LEN];
    char categories[MAX_CATEGORIES][MAX_STRING_LEN];
    int numCategories;
    int parent;
    int size;
};

/**
 * Task 1
 * Returned list should contain copies of the names e.g. through strdup()
 */
char **leafFiles(struct File *files, int numFiles, int *numLeafFiles) {

    *numLeafFiles = 0;

    int *parentIds = malloc(numFiles * sizeof(int));
    int count = 0;
    int parentId = 0;

    for (int j = 0; j < numFiles; j++) {
        parentIds[j] = files[j].parent;
    }
    
    char **leafFileNames = malloc(numFiles * sizeof(char *));
    for (int i = 0; i < numFiles; i++) {
        int isLeaf = 1;
        for (int k = 0; k < numFiles; k++) {
            if (files[i].id == parentIds[k]) {
                isLeaf = 0;
                break; 
            }
        }
        if (isLeaf == 1) {
            leafFileNames[(*numLeafFiles)++] = strdup(files[i].name);
        }
    }

    free(parentIds);
    
    return leafFileNames;
}

/**
 * Task 2
 * Returned list should contain copies of the categories e.g. through strdup()
 */
int sortCompareFunc(const void *p, const void *q);

// struct which has category name and the its count
struct tempCategory {
    char name[MAX_STRING_LEN];
    int count; 
};

char **kLargestCategories(struct File *files, int numFiles, int k, int *numReturned) {
    *numReturned = 0;

    struct tempCategory *categoryArray = malloc(MAX_CATEGORIES * sizeof(struct tempCategory));
    int countedCategories = 0;

    for (int i = 0; i < numFiles; i++) {
        for (int j = 0; j < files[i].numCategories; j++) {
            int flag = 0;
            for (int k = 0; k < countedCategories; k++) {
                if (strcmp(categoryArray[k].name, files[i].categories[j]) == 0) { 
                    categoryArray[k].count++;
                    flag = 1; // flag to check when the catagory in the array of categories is found
                    break;
                }
            }
            if (flag == 0) {
                strcpy(categoryArray[countedCategories].name, files[i].categories[j]);
                categoryArray[countedCategories].count = 1;
                countedCategories++;
            }
        }
    }

    char **kCategories = malloc(k * sizeof(char *));

    qsort(categoryArray, countedCategories, sizeof(struct tempCategory), sortCompareFunc);
    // Copy the categories to the returned list
    for (int i = 0; i < k && i < countedCategories; i++) {
        kCategories[i] = strdup(categoryArray[i].name);
        (*numReturned)++;
    }

    //printf("testing %d\n", *numReturned);

    free(categoryArray);
    return kCategories;
}

int sortCompareFunc(const void *p, const void *q) {
    const struct tempCategory *tempCategoryp = (const struct tempCategory *)p;
    const struct tempCategory *tempCategoryq = (const struct tempCategory *)q;

    if (tempCategoryp->count > tempCategoryq->count) {
        return -1; // Sort by count in descending order
    } else if (tempCategoryp->count < tempCategoryq->count) {
        return 1;
    } 
    
    return strcmp(tempCategoryp->name, tempCategoryq->name); // Sort by name in ascending order
    
}

/**
 * Task 3
 */
int getFileSize(struct File *files, int numFiles, int fileId);

int largestFileSize(struct File *files, int numFiles) {
    int maxSize = 0;
    for (int i = 0; i < numFiles; i++) {
        int totalSize = files[i].size + getFileSize(files, numFiles, files[i].id);
        if (totalSize > maxSize) {
            maxSize = totalSize;
        }
    }
    return maxSize;
}

int getFileSize(struct File *files, int numFiles, int fileId) {
    int size = 0;
    for (int i = 0; i < numFiles; i++) {
        if (files[i].parent == fileId) { // if looping through files and parent id = original id passed in 
                                            // then found child file and update size 
            size += files[i].size + getFileSize(files, numFiles, files[i].id);
        }
    }
    return size;
}


int qsortStrcmp(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

int main(void) {
    struct File testFiles[] = {
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Media", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 4096 },
        { .id = 8, .name = "Backup.zip", .categories = {"Backup"}, .numCategories = 1, .parent = 233, .size = 8192 },
        { .id = 13, .name = "Presentation.pptx", .categories = {"Documents", "Presentation"}, .numCategories = 2, .parent = 3, .size = 3072 },
        { .id = 21, .name = "Video.mp4", .categories = {"Media", "Videos"}, .numCategories = 2, .parent = 34, .size = 6144 },
        { .id = 34, .name = "Folder2", .categories = {"Folder"}, .numCategories = 1, .parent = 3, .size = 0 },
        { .id = 55, .name = "Code.py", .categories = {"Programming"}, .numCategories = 1, .parent = -1, .size = 1536 },
        { .id = 89, .name = "Audio.mp3", .categories = {"Media", "Audio"}, .numCategories = 2, .parent = 34, .size = 2560 },
        { .id = 144, .name = "Spreadsheet2.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 2048 },
        { .id = 233, .name = "Folder3", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 4096 },
    };

    struct File testFiles1[] = {
        { .id = 1, .name = "a", .categories = {"a1"}, .numCategories = 1, .parent = 3, .size = 1 },
        { .id = 2, .name = "b", .categories = {"a1", "b1"}, .numCategories = 2, .parent = 3, .size = 2 },
        { .id = 3, .name = "c", .categories = {"a1", "b1", "c1"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 4, .name = "d", .categories = {"b1", "c1"}, .numCategories = 2, .parent = 3, .size = 4 },
    };

    int numLeafFiles;
    char *expectedLeafFiles[] = {
        "Audio.mp3",
        "Backup.zip",
        "Code.py",
        "Document.txt",
        "Image.jpg",
        "Presentation.pptx",
        "Spreadsheet.xlsx",
        "Spreadsheet2.xlsx",
        "Video.mp4"
    };
    char **returnedLeafFiles = leafFiles(testFiles, 12, &numLeafFiles);
    qsort(returnedLeafFiles, numLeafFiles, sizeof(char *), &qsortStrcmp);

    /*
    for (int j = 0; j < 9; j++) {
        printf("list %s\n", returnedLeafFiles[j]);
    }
    printf(" number leafs %d\n", numLeafFiles);
    */

    assert(numLeafFiles == 9);
    for (int i = 0; i < 9; i++) {
        assert(strcmp(returnedLeafFiles[i], expectedLeafFiles[i]) == 0);
        free(returnedLeafFiles[i]);
    }
    free(returnedLeafFiles);

    //more leaf tests
    char *expectedLeafFiles1[] = {
        "a",
        "b",
        "d"
    };
    char **returnedLeafFiles1 = leafFiles(testFiles1, 4, &numLeafFiles);
    qsort(returnedLeafFiles1, numLeafFiles, sizeof(char *), &qsortStrcmp);
    assert(numLeafFiles == 3);
    for (int i = 0; i < 3; i++) {
        assert(strcmp(returnedLeafFiles1[i], expectedLeafFiles1[i]) == 0);
        free(returnedLeafFiles1[i]);
    }
    free(returnedLeafFiles1);

    int numCategories;
    char *expectedCategories[] = {"Documents", "Folder", "Media"};
    char **returnedCategories = kLargestCategories(testFiles, 12, 3, &numCategories);

   /*
    printf(" number categories %d\n", numCategories);
    for (int i = 0; i < 10; i++) {
        printf("list %s\n", returnedCategories[i]);
    }
    */

    assert(numCategories == 3);
        for (int i = 0; i < 3; i++) {
        assert(strcmp(returnedCategories[i], expectedCategories[i]) == 0);
        free(returnedCategories[i]);
    }
    free(returnedCategories);

    //more category tests
    char *expectedCategories1[] = {"a1", "b1"};
    char **returnedCategories1 = kLargestCategories(testFiles1, 4, 2, &numCategories);
    assert(numCategories == 2);
        for (int i = 0; i < 2; i++) {
        assert(strcmp(returnedCategories1[i], expectedCategories1[i]) == 0);
        free(returnedCategories1[i]);
    }
    free(returnedCategories1);

    assert(largestFileSize(testFiles, 12) == 20992);
    assert(largestFileSize(testFiles1, 4) == 7);

}