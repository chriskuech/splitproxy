
function cleanup {
    pkill testserver
    pkill splitproxy
    sleep 1
}

# start servers
cleanup
./testserver 8001 > ./test/out1.txt &
./testserver 8002 > ./test/out2.txt &
./splitproxy 8000 127.0.0.1:8001 127.0.0.1:8002 &

# run client
./testclient 127.0.0.1:8000 < ./test/in.txt
sleep 1
cleanup

# validate
DIF=$(diff ./test/out1.txt ./test/out2.txt)
if [[ $DIF ]]; then
    echo -e "\n$DIF\n"
else
    echo -e "\nTest passes\n"
fi
