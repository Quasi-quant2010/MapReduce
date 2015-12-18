# 1. Install Kytea for Python Wrapper
- Before python wrapper kytea, you must install kytea.<br>
If Mykytea-python not installed, you can not carray out alice/wc_demo_local.py.
- cd Mykytea-python
- swig -c++ -python -I/usr/local/include  mykytea_out2.i
- g++ -c mykytea_out2_wrap.cxx -I/home/username/.pythonbrew/pythons/Python-2.7.3/include/python2.7  -fPIC
- g++ -c mykytea_out2.cpp -I/usr/local/include  -L/usr/local/lib -fPIC
- g++ -shared mykytea_out2.o mykytea_out2_wrap.o -o _Mykytea.so -lkytea

# 2. Local Computing for debug
cat alice/test_alice.txt | python alice/wc_demo_local.py map | python alice/wc_demo_local.py combine | python alice/wc_demo_local.py reduce

# 3. Hadoop Cluster Computing
- 3.1 Make Tar File <br>
In this sample, we alreday make the tar file, test_alice.tar, by pyinstaller. <br>
You also read the hadoopy document.

- 3.2 Put Tar file into HDFS for distrbuted cache <br>
hadoop fs -put test_alice.tar opt

- 3.3 Put data file into HDFS <br>
hadoop fs -put test_alice.txt opt/input <br>
(If you have typedbytes file, you can out it.)

- 3.4 MapReduce <br>
hadoop jar $streaming_path <br>
       -output opt/out <br>
       -outputformat org.apache.hadoop.mapred.SequenceFileOutputFormat <br>
       -input opt/test_alice.txt <br>
       -inputformat AutoInputFormat <br>
       -mapper "_frozen/wc_demo pipe map" <br>
       -reducer "_frozen/wc_demo pipe reduce" <br>
       -combiner "_frozen/wc_demo pipe combine" <br>
       -io typedbytes <br>
       -file model.bin <br>
       -jobconf "mapred.cache.archives=opt/test_alice.tar#_frozen" <br>
       -jobconf "mapred.map.tasks=5" <br>
       -jobconf "mapred.reduce.tasks=1" <br>
       -jobconf "mapred.job.name=wc_demo" <br>
       -jobconf "mapreduce.job.cache.archives=opt/test_alice.tar#_frozen" <br>
       -jobconf "mapred.map.output.compression.codec=org.apache.hadoop.io.compress.SnappyCodec" <br>
       -jobconf "mapred.compressmap.output=true" <br>
       -jobconf "mapred.compress.map.output=true" <br>
       -jobconf "mapred.output.compress=true" <br>
       -jobconf "mapred.output.compression.codec=org.apache.hadoop.io.compress.SnappyCodec";
