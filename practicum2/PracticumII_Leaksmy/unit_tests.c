#include <stdio.h>
#include <unistd.h>
#include "commonfunction.h"

void test_executing_write_command() {
    /**
     * Unit test for WRITE execution
     * */
     printf("\n\n .....Test WRITE Command..... \n\n");
     char *local_test1 = "local_file/photo.xml";
     char *local_test2 = "local_file/test.c";
     char *local_test3 = "local_file/test.txt";
     char *local_test4 = "local_file/local_file_level/test.txt";
     char *local_test5 = "local_file/test.txt";

     char *remote_test1 = "photo.xml";
     char *remote_test2 = "test/test.c";
     char *remote_test3 = "test/test.txt";
     char *remote_test4 = "folder1/folder2/folder3/copy_file.txt";

     // test if xml file is copied correctly
     // copy from local_file/photo.xml to remote_file_path/photo.xml
     executing_write_command(local_test1, remote_test1);

     // test different file type. Now is .c
     // copy from local_file/test.c to remote_file_path/local_file/test.c
     executing_write_command(local_test2, remote_test2);

     // test different file type. Now is .txt
     executing_write_command(local_test3, remote_test3);

     // test when local_file go in-dept and remotefile is also go indept
     executing_write_command(local_test4, remote_test4);

     // test when remote_test is not specified
     executing_write_command(local_test5, local_test5);

     /**
      * Execute newer version
      * */
     // now you should see photo.xml, photo_v1.xml, photo_v2.xml
     // where v1 is the oldest content, v2 is the next oldest so it has photo_a.xml there
     // photo.xml will have photo_b.xml content there
     char *local_test1_a = "local_file/photo_a.xml";
     char *local_test3_b = "local_file/photo_b.xml";
     executing_write_command(local_test1_a, remote_test1);
     executing_write_command(local_test3_b, remote_test1);

     /**
      * Execute when local file not exist
      * */
      char *not_test1 = "local_file/photo_notexist.xml";
      char *not_test2 = "local_file/test_notexist.c";
      executing_write_command(not_test1, local_test5);
      executing_write_command(not_test2, not_test2);
}


void test_executing_get_command() {
    /**
     * Unit test for GET execution
     * */
     printf("\n\n .....Test GET Command..... \n\n");
     char *local_test1 = "local_file/photo.xml";
     char *local_test2 = "local_file/test.c";
     char *local_test3 = "local_file/test.txt";
     char *local_test4 = "local_file/local_file_level/level2/test.txt";
     char *local_test5 = "local_file/test.txt";

     char *remote_test1 = "photo.xml";
     char *remote_test2 = "test/test.c";
     char *remote_test3 = "test/test.txt";
     char *remote_test4 = "folder1/folder2/folder3/copy_file.txt";

     // test that the remote_test will go to the correct remote directory eventhough
     // we only specified photo.xml without file path
     executing_get_command(remote_test1, local_test1);

     // test that it fines the path correctly with one level below (like in test/ directory)
     // test different file format too
     executing_get_command(remote_test2, local_test2);

     // test regular working correctly
     executing_get_command(remote_test3, local_test3);

     // test multiple directory below ini both remote and local file
     executing_get_command(remote_test4, local_test4);

     // test writing to the same directory in local with the same file name
     // this expect to overwrite unlike in remote where we have version control
     executing_get_command(remote_test4, local_test5);

     // test when there is no file in remote
     char *file_not_found1 = "file_not_found/test/test.txt";
     char *file_not_found2 = "test_not_found.txt";
     executing_get_command(file_not_found1, local_test5);
     executing_get_command(file_not_found2, local_test5);
}


void test_executing_ls_command() {
    /**
     * Unit test for LS execution
     * */
    printf("\n\n .....Test ls Command..... \n\n");

    char *remote_test1 = "photo.xml";
    char *remote_test2 = "test/test.c";
    char *remote_test3 = "test/test.txt";
    char *remote_test4 = "nothingfound/folder1/folder2/folder3/copy_file.txt";

    // photo has multiple version,so you'll be able to see all versions are listed in there with information
    executing_ls_command(remote_test1);

    // just one is found
    executing_ls_command(remote_test2);
    executing_ls_command(remote_test3);

    // nothing is found
    executing_ls_command(remote_test4);
}


void test_executing_remove_command() {
    /**
     * Unit test for REMOVE execution
     * Sleep each remove so you can see that the file is missing from remote path
     * */
    printf("\n\n .....Test RM Command..... \n\n");
    char *remote_test1 = "photo.xml";
    char *remote_test2 = "test/test.c";
    char *remote_test3 = "test/test.txt";
    char *remote_test4 = "folder1/folder2/folder3/copy_file.txt";
    char *remote_test7 = "not_exist.txt";
    executing_remove_command(remote_test1);
    sleep(3);
    executing_remove_command(remote_test2);
    sleep(3);
    executing_remove_command(remote_test3);
    sleep(3);
    executing_remove_command(remote_test4);
    sleep(3);

    // execute file that does not exist
    executing_remove_command(remote_test7);
}


void main() {
    test_executing_write_command();
    test_executing_get_command();
    test_executing_ls_command();
    test_executing_remove_command();
}