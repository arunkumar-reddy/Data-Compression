echo "Compressing the image and Encoding the image"
g++ `Magick++-config --cxxflags --cppflags` -O2 encode.cpp `Magick++-config --ldflags --libs` -o encode
./encode $1
input="$2"
zip=${input/jpg/zip}
zip -m ${zip} size.txt yac.txt ydc.txt yactree.txt ydctree.txt Cbac.txt Cbdc.txt Cbactree.txt Cbdctree.txt Crac.txt Crdc.txt Cractree.txt Crdctree.txt
unzip ${zip} size.txt yac.txt ydc.txt yactree.txt ydctree.txt Cbac.txt Cbdc.txt Cbactree.txt Cbdctree.txt Crac.txt Crdc.txt Cractree.txt Crdctree.txt
g++ `Magick++-config --cxxflags --cppflags` -O2 decode.cpp `Magick++-config --ldflags --libs` -o decode
./decode $2
rm size.txt yac.txt ydc.txt yactree.txt ydctree.txt Cbac.txt Cbdc.txt Cbactree.txt Cbdctree.txt Crac.txt Crdc.txt Cractree.txt Crdctree.txt
echo "Decoding the image" 
echo "Compressed image has been created"
