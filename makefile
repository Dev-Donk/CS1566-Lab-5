# -lXi -lXmu assuming these were needed for WSL?

LIBS = -lglut -lGLEW -lGLU -lm -lGL

lab5: lab5.c myLib.o initShader.o
	gcc -o lab_5 lab5.c myLib.o initShader.o $(LIBS)

lab5_ctm: lab5_ctm.c initShader.o
	gcc -o lab5_ctm lab5_ctm.c initShader.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.c myLib.h
	gcc -c myLib.c
