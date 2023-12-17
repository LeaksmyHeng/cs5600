#! /bin/bash
# executing the c script
gcc helper-arger-heng.c arger-heng.c -o arger-heng

# grant permission to arger-heng file
sudo chmod +x arger-heng


echo ""
echo "Test all upper case"
./arger-heng -u i Love You 3000 from Iron Man
./arger-heng -u ^_^ Hello My name is leaksmy heng
./arger-heng -u ,. Hello have a good day 
./arger-heng -u Today is the 17 of Sep :D

echo ""
echo "Test all lower case"
./arger-heng -l i Love You 3000 from Iron Man
./arger-heng -l ^_^ Hello My name is leaksmy heng
./arger-heng -l ,. Hello have a good day 
./arger-heng -l Today is the 17 of Sep :D

echo ""
echo "Test capital letter"
./arger-heng -cap i Love You 3000 from Iron Man
./arger-heng -cap ^_^ Hello My name is leaksmy heng
./arger-heng -cap ,. Hello have a good day 
./arger-heng -cap Today is the 17 of Sep :D

echo ""
echo "Test error when there is no -u or -i or -cap"
./arger-heng -capital i Love You 3000 from Iron Man
./arger-heng ^_^ Hello My name is leaksmy heng
./arger-heng ehhhh ,. Hello have a good day 
./arger-heng -z Today is the 17 of Sep :D

echo ""
echo "Test error no argument input"
./arger-heng
./arger-heng -u
./arger-heng -l 
./arger-heng -cap

echo ""
echo "Test error the first argument is not ./arger-heng"
gcc helper-arger-heng.c arger-heng.c -o naruto
./naruto -u i Love You 3000 from Iron Man
./naruto ^_^ Hello My name is leaksmy heng
./naruto -cap ehhhh ,. Hello have a good day 
./naruto -l Today is the 17 of Sep :D
