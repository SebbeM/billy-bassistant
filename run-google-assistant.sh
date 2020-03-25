#! /bin/bash

# Make sure that a connection to Google can be established before running the assistant
check_google () {
  netcat -z -w 5 8.8.8.8 53 && echo 1 || echo 0
}

until [ `check_google` -eq 1 ]; do
  sleep 2
done

cd /home/pi/billy-bassistant/googleassistant/
. env/bin/activate
./env/bin/python3 env/lib/python3.7/site-packages/googlesamples/assistant/grpc/pushtotalk.py &
