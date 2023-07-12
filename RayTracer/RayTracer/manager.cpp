#include "manager.h"

std::vector<glm::vec3> readScreenData(FILE* file) {

	char line[100];
	int dir[3][3];
	std::vector<glm::vec3> cameraPos(3);

	for(int i = 0; i < SCREEN_DATA_SIZE - 1; i++){
		fgets(line, sizeof(line), file);
		std::stringstream str(line);
		str >> dir[i][0] >> dir[i][1] >> dir[i][2];
	}
	
	cameraPos[0] = glm::vec3(dir[0][0], dir[0][1], dir[0][2]);
	cameraPos[1] = glm::vec3(dir[1][0], dir[1][1], dir[1][2]);
	cameraPos[2] = glm::vec3(dir[2][0], dir[2][1], dir[2][2]);

	fgets(line, sizeof(line), file);
	int normal = atoi(line);
	
	return cameraPos;
}


glm::mat4 getView(std::vector<glm::vec3> pos) {
	glm::mat4 view = glm::lookAt(
		pos[0],
		pos[1],
		pos[2]
	);
	return view;
}


void readLightData(FILE* file){

	char line[100];
	fgets(line, sizeof(line), file);
	int n = atoi(line);

    if (n < 1) throw std::runtime_error("ERROR: Argumento para inicialização da iluminação inválidos.\n");

	float pos[10][3];
	float col[10][3];
	float atn[10][3];

	for(int i = 0; i < n; i++){
		fgets(line, sizeof(line), file);
		std::stringstream str(line);
		str >> pos[i][0] >> pos[i][1] >> pos[i][2] 
			>> col[i][0] >> col[i][1] >> col[i][2] 
			>> atn[i][0] >> atn[i][1] >> atn[i][2];
	}



    GLfloat lightPosition[] = { pos[0][0], pos[0][1], pos[0][2], 0.0f };  // XYZ values (0.0f for positional light)
    GLfloat lightColor[] = { col[0][0], col[0][1], col[0][2], 1.0f };  // RGBA values

	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	/* FALTA AS OUTRAS LUZES */

}


std::vector<Color> readColorData(FILE* file){

	char line[100];
	fgets(line, sizeof(line), file);
	int n = atoi(line);

    if (n < 1) throw std::runtime_error("ERROR: Argumento para inicialização das cores inválidos.\n");

	std::vector<Color> colorVec(n);

	for(int i = 0; i < n; i++){

		fgets(line, sizeof(line), file);
		std::stringstream input(line);

		std::string str;
		input >> str;
		
    	size_t spacePos = str.find(' ');
		std::string colorType = str.substr(0, spacePos);

		Color color;
		color.colorType = colorType;
		color.colorId = i;

		if(colorType == COL_TEXMAP){

			std::string colorFile = str.substr(spacePos+1, str.size()-1);

			fgets(line, sizeof(line), file); 
			std::stringstream input1(line);
			input1 >> color.vec4[0] >> color.vec4[1] >> color.vec4[2] >> color.vec4[3];

			fgets(line, sizeof(line), file);
			std::stringstream input2(line);
			input2 >> color.vec3[0] >> color.vec3[1] >> color.vec3[2];

			color.colorFile = colorFile;

		} else if(colorType == COL_CHECKER) {

			input >> str 
				  >> color.col1[0] >> color.col1[1] >> color.col1[2]
				  >> color.col2[0] >> color.col2[1] >> color.col2[2]
				  >> color.lenght;

		} else if(colorType == COL_SOLID){

			input >> str >> color.col1[0] >> color.col1[1] >> color.col1[2];
			
		} else{
			throw std::runtime_error("ERROR: Cor inexistente.\n");
		}

		colorVec[i] = color;
	}

	return colorVec;

}


std::vector<Texture> readTextureData(FILE* file){

	char line[100];
	fgets(line, sizeof(line), file);
	int n = atoi(line);

    if (n < 1) throw std::runtime_error("ERROR: Argumento para inicialização dos corpos.\n");

	std::vector<Texture> textureVec(n);

	for(int i = 0; i < n; i++){
		
		fgets(line, sizeof(line), file);
		std::stringstream input(line);

		Texture texture;
		texture.textureId = i;

		input >> texture.ka >> texture.kd >> texture.ks >> texture.alfa
			  >> texture.kr >> texture.kt >> texture.ior;
		
		textureVec[i] = texture;
	}

	return textureVec;
}


std::vector<Body> readBodyData(FILE* file, std::vector<Color> colorVec, std::vector<Texture> textureVec){

	char line[100];
	fgets(line, sizeof(line), file);
	int n = atoi(line);

    if (n < 1) throw std::runtime_error("ERROR: Argumento para inicialização dos corpos.\n");

	std::vector<Body> bodyVec(n);

	for(int i = 0; i < n; i++){
		
		fgets(line, sizeof(line), file);
		std::stringstream input(line);

		int colorId, textureId;
		input >> colorId >> textureId;

		Body body;
		body.color = colorVec[colorId];
		body.texture = textureVec[textureId];

		std::string objType;
		input >> objType;

		if(objType == SPHERE){

			Sphere sphere;
			input >> sphere.pos[0] >> sphere.pos[1] 
				  >> sphere.pos[2] >> sphere.radius;
			body.sphere = sphere;

		} else if (objType == POLYHEDRON) {

			Polyhedron polyhedron;
			int n = 0;
			input >> n;
			if(n > 20) throw std::runtime_error("ERROR: Número de vértices inválido.\n");
			
			polyhedron.n = n;
			for(int i = 0; i < n; i++){
				input >> polyhedron.side[i][0] >> polyhedron.side[i][1] 
					  >> polyhedron.side[i][2] >> polyhedron.side[i][3];
			}
			body.polyhedron = polyhedron;

		} else{
			throw std::runtime_error("ERROR: Objeto não reconhecido.\n");
		}

		bodyVec[i] = body;
	}

	return bodyVec;
}



void drawScene(std::vector<Body> bodyVec){
	
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Set color to red
        glVertex3f(-0.6f, -0.4f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);  // Set color to green
        glVertex3f(0.6f, -0.4f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);  // Set color to blue
        glVertex3f(0.0f, 0.6f, 0.0f);
        glEnd();
}