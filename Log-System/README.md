# BI-PROJECT

This is the final project of BI course.



### Get flume and kafka service

Download from the website:

- [flume](https://flume.apache.org/)
- [kafka](https://kafka.apache.org/)



### Start the project

```bash
# zookeeper
bin/zookeeper-server-start.sh config/zookeeper.properties

# kafka
bin/kafka-server-start.sh config/server.properties

# flume
bin/flume-ng agent --conf conf --conf-file job/flume-kafka.conf --name agent -Dflume.root.logger=INFO,console

# fastapi
uvicorn app:app --host 0.0.0.0 --port 8000
```

Finally start the `consumer.py` script.




### Other command

```bash
# create a kafka topic
bin/kafka-topics.sh --create --topic click_logs --bootstrap-server localhost:9092 --partitions 1 --replication-factor 1

# delete a kafka topic
bin/kafka-topics.sh --bootstrap-server localhost:9092 --delete --topic click_logs

# check kafka topic
bin/kafka-topics.sh --bootstrap-server localhost:9092 --list
```