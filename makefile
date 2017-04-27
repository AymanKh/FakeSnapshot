fakeSnapshot: MakeFakeSnapshot.c
	gcc  -o fromCtoJava -I/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/include -I/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/include/darwin -L/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/server/ MakeFakeSnapshot.c -ljvm
