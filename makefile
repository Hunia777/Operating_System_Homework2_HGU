all : pctest FailureDetect tester

pctest : pctest.c
	gcc pctest.c -o pctest

FilureDetect : FailureDetect.c
	gcc FailureDetect.c -o FailureDetect

tester : tester.c
	gcc tester.c -o tester

clean:
	rm FailureDetect pctest tester solution_results.txt target_results.txt
