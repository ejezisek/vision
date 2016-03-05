#NOTE THIS CAN ONLY BE RUN IF SH IS INSTALLED.  SH IS EASIEST TO RUN FROM A LINUX BOX BUT CAN BE RUN THROUGH CYGWIN

cd ../RemoveBackground
make
cd ../Testing
mainScript="../RemoveBackground/BackgroundRemover"
folder="Input/"
files="TestInput1.avi TestInput2.avi TestInput3.avi TestInput4.avi TestInput5.avi"
for i in $files; do
	$mainScript $folder$i
done
