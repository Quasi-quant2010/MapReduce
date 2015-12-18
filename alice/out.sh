# !/bin/sh

hdfs_base="hdfs://nameservice1/user/opt";
input_hdfs_path='playground/test_alice.txt';
output_hdfs_path='playground/out';
streaming_path="/usr/lib/hadoop-mapreduce/hadoop-streaming.jar";

hadoop fs -test -e ${hdfs_base}"/"${output_hdfs_path};
tmp_bool=$(echo $?);
if [ ${tmp_bool} -eq 0 ];
then
    hadoop fs -rmr ${hdfs_base}"/"${output_hdfs_path};
fi

hadoop fs -test -e ${hdfs_base}"/"${input_hdfs_path};
tmp_bool=$(echo $?);
if [ ${tmp_bool} -eq 0 ];
then
    hadoop jar ${streaming_path} \
	-output ${output_hdfs_path} \
	-outputformat org.apache.hadoop.mapred.SequenceFileOutputFormat \
	-input ${input_hdfs_path} \
	-inputformat AutoInputFormat \
	-mapper "_frozen/wc_demo pipe map" \
	-reducer "_frozen/wc_demo pipe reduce" \
	-combiner "_frozen/wc_demo pipe combine" \
	-io typedbytes \
	-file model.bin \
	-jobconf "mapred.cache.archives=opt/test_alice.tar#_frozen" \
	-jobconf "mapred.map.tasks=5" \
	-jobconf "mapred.reduce.tasks=1" \
	-jobconf "mapred.job.name=wc_demo" \
	-jobconf "mapreduce.job.cache.archives=opt/test_alice.tar#_frozen" \
	-jobconf "mapred.map.output.compression.codec=org.apache.hadoop.io.compress.SnappyCodec" \
	-jobconf "mapred.compressmap.output=true" \
	-jobconf "mapred.compress.map.output=true" \
	-jobconf "mapred.output.compress=true" \
	-jobconf "mapred.output.compression.codec=org.apache.hadoop.io.compress.SnappyCodec";
fi


