#! /bin/bash
# executing the c script

# execute client
gcc client.c commonfunction.c -o rfs
echo ""
echo ".....Test WRITE Command....."
./rfs WRITE local_file/photo.xml photo.xml
./rfs WRITE local_file/test.c test/test.c
./rfs WRITE local_file/test.txt test/test.txt
./rfs WRITE local_file/local_file_level/level2/test.txt folder1/folder2/folder3/copy_file.txt

echo ".....Test WRITE Command while file not found....."
./rfs WRITE file_not_found/test/test.txt file_not_found/test/test.txt


echo ""
echo ".....Test GET Command....."
./rfs GET photo.xml local_file/copy/photo.xml
./rfs GET test/test.c test/copy/test.c
./rfs GET test/test.txt test/copy/test.txt
./rfs GET folder1/folder2/folder3/copy_file.txt  test/copy.copy_file.txt


echo ""
echo ".....Test LS Command....."
./rfs LS photo.xml
./rfs LS test/test.c
./rfs LS nothingfound/folder1/folder2/folder3/copy_file.txt

echo ""
echo ".....Test RM Command....."
/rfs rm photo.xml
/rfs rm test/test.c
/rfs rm test/test.txt
/rfs rm folder1/folder2/folder3/copy_file.txt


echo ""
echo ".....Test Sending WRITE Command simutaneously....."
# we would expect to see 2 files for thread1 and thread2
./rfs WRITE local_file/thread1.txt thread1.txt &
./rfs WRITE local_file/thread2.txt thread2.txt &
./rfs WRITE local_file/thread1.txt thread1.txt &
./rfs WRITE local_file/thread2.txt thread2.txt &

wait
echo "Thread1 and thread2.txt run twice in parrallel so we should see 4 files in total"
