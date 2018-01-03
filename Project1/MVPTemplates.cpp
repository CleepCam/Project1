#include "MVPTemplates.h"

MVPTemplates::MVPTemplates()
{
	error = roc_initialize("C:\\ROC-Windows\\ROC.lic", NULL );
	if (error) {
		cerr << error;
		exit(EXIT_FAILURE);
	}
	index = -1;
}
MVPTemplates::~MVPTemplates(){
	error = roc_finalize();
	if (error) {
		cerr <<"Error closing roc: "<< error;
	}
}

bool MVPTemplates::convertImageToTemplate() {
	bool isSuccessful = true;
	// Initialize SDK
		// Open  one image.
	roc_algorithm_id id = ROC_FRONTAL |ROC_PAD| ROC_ROLL | ROC_FR 
						| ROC_DEMOGRAPHICS | ROC_GLASSES | ROC_LANDMARKS;
	mvpTemplate = (roc_template*)malloc(MAX_FACES_TO_DETECT * sizeof(roc_template));

	error = roc_read_image(imageFileName, ROC_BGR24, &images);
	if (error) {
		cerr << "Roc Error: " << error << endl;
		return false;
	}
	else {
		error =roc_adaptive_minimum_size(images, 0.08f, 36, &adaptive_minimum_size);
		if (error) {
			cerr <<"Roc error occured in getting adaptive minimum size of image: "<< error<<endl;
			adaptive_minimum_size = 36;
		}
		error = roc_represent(images, id, 
			adaptive_minimum_size, 2, 0.20f, mvpTemplate);
			if (error) {
			cerr << error;
			isSuccessful = false;
		}
			else {
				if (!(mvpTemplate[1].algorithm_id & ROC_INVALID)) {
					cerr << "Error! Multiple faces Detected! Please enroll single face picture!"<<endl;
					cleanUp();
					return false;
				}
				else {
					if (mvpTemplate[0].algorithm_id & ROC_INVALID) {
						cerr << "Roc Error, Failed to detect face in image: " << imageFileName << "\n";
						isSuccessful = false;
					}
			}
		}
		roc_free_image(images);
		return isSuccessful;
	}
}

void MVPTemplates::cleanUp()
{
	for (auto j = 0; j <MAX_FACES_TO_DETECT; j++) {
		if (!(mvpTemplate[j].algorithm_id & ROC_INVALID)) {
			roc_free_template(&mvpTemplate[j]);
		}
		else break;
	}

}

size_t MVPTemplates::enrollTemplate(char* imageFile, char * myGallFile) {
	gallFile = myGallFile;
	this->imageFileName = imageFile;

	if (!convertImageToTemplate())return -1;

	else {
		//exit if gallery file cannot be open
		error = roc_open_gallery(gallFile, &mvpGallery, NULL);
		if (error) {
			cerr << "Roc Error, failed to open gallery file for given index: " << error << endl;
			return -1;
		}
		roc_size(mvpGallery, &index);// get the index number of the template to be enrolled
		//if index is greater than -1
		if (index >= 0 && index<10) {
			error = roc_enroll(mvpGallery, mvpTemplate[0]);//write template in galery
			//if writing fails reset index to -1;
			if (error) {
				cerr << "Roc Error, failed to enroll face: "<<error<<endl;
				index = -1;
			}
			else {
				roc_size(mvpGallery, &index);
				cout << "Enroll to gallery successful!" << endl;
				cleanUp();
			}
		}
		else{
			cerr << "Enroll failed!!!\n";
			index= -1;
		}
		error = roc_close_gallery(mvpGallery);
		if (error) {
			cerr <<"Roc Error in closing gallery: "<< error<<endl;
		}
			return index;
	}
}
