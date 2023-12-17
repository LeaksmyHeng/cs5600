#! /bin/bash
# executing the c script
make

echo ""
echo "Test encoding working correctly"
./cipher -e "i Love You 3 from Iron Man"
./cipher -e "Hello My name is leaksmy heng"
./cipher -e ",. Hello have a good day"
./cipher -e "Today is the 17 of Sep :D"

echo ""
echo "Test decoding working correctly"
./cipher -d "24 31345115 543445   21423432 24423433 321133"
./cipher -d "2315313134 3254 33113215 2443 31151125433254 23153322"
./cipher -d "   2315313134 23115115 11 22343414 141154"
./cipher -d "4434141154 2443 442315    3421 431535  14  "

echo ""
echo "Invalid tag"
./cipher -q "i Love You 3 from Iron Man"
./cipher -i "Hello My name is leaksmy heng"
./cipher -p ",. Hello have a good day"
./cipher -w "Today is the 17 of Sep :D"

