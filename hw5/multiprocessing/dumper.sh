#! /bin/bash
# executing the c script
make

echo ""
echo "run normally. No input file or output file name is provided"
./multiprocess
echo "check your folder now; there is around 90+ files in there as we generate batch"
sleep 10

echo ""
echo "run the output_batch1 cause we know its output file will be the same as that file cause we cannot cipher number."
./multiprocess "output_batch1.txt" "out_batch1"
echo "check your folder now for out_batch1_batch1; the content is the same as your output_batch1.txt"
sleep 10


echo ""
echo "Testing empty input file now"
touch empty.txt
./multiprocess "empty.txt" "empty"
echo "As expected there is no empty_batch1.txt file as the output cause our file is empty so it can't even write to the queue in the beginning"
sleep 10

echo ""
echo "Testing when input file cannot be found"
./multiprocess "youcannotfindme"
echo "when file is not found our program is expected to call the default file and overwrite the previous files"
sleep 10


echo ""
echo "Testing file with special character"
./multiprocess "input_file_special_char.txt" "special_char"
echo "There should be special_char_batch1 in the folder now"
sleep 10
