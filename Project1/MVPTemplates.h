#pragma once
#include <iostream>
#include <roc.h>

#define EXIT_FAILURE -1
#define MAX_FACES_TO_DETECT 2


using namespace std;
class MVPTemplates
{

private:
	roc_gallery mvpGallery;
	roc_template *mvpTemplate;
	roc_gallery_file gallFile;
	roc_image images;
	size_t index;
	char *imageFileName;
	roc_error error;
	size_t adaptive_minimum_size;
	
	

public:
	MVPTemplates();
	bool convertImageToTemplate();
	void cleanUp();
	size_t enrollTemplate(char*, char*);
	~MVPTemplates();
};

