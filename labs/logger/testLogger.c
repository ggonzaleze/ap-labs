int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main() {
	int n=1;
	infof("I am INFO number %d \n",n);
	warnf("I am WARNING number %d \n",n);
	errorf("I am ERROR number %d \n",n);
	panicf("I am PANIC number %d \n",n);
	return 0;
}
