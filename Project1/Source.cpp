#include "MVPTemplates.h"

using namespace std;

int main(int argc, char *argv[])
{
	MVPTemplates temp;
	size_t index;
	float match = 0.0;

	if (argc != 3) {
		puts("Expected two arguments:\n"
		"image path, galleryfile path\n");
		//temp.enrollTemplate("C:\\CleepCam-MVP\\Backend\\mikey.png", "C:\\CleepCam-MVP\\Backend\\you.txt");
		//system("pause");
		return EXIT_FAILURE;
	}

	else {
		index = temp.enrollTemplate(argv[1], argv[2]);

		if (index < 0) {
			return EXIT_FAILURE;
		}

		else {
			cout << "Enroll Action was successful, Thank you!" << endl;
			return EXIT_SUCCESS;
		}
	}
}