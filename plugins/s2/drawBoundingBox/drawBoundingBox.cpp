#include "drawBoundingBox.h"
#include "glwidget.h"

void DrawBoundingBox::onPluginLoad()
{
	QString vs_src =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vertex;"
	"layout (location = 2) in vec3 color;"
	"uniform mat4 modelViewProjectionMatrix;"
	"uniform vec3 boundingBoxMin;"
	"uniform vec3 boundingBoxMax;"
	"out vec4 frontColor;"
	"void main()"
	"{"
	"    frontColor = vec4(color, 1.0);"
	"    mat4 scale = mat4(vec4(boundingBoxMax.x - boundingBoxMin.x, 0, 0, 0),"
	"			vec4(0, boundingBoxMax.y - boundingBoxMin.y, 0, 0),"
	"			vec4(0, 0, boundingBoxMax.z - boundingBoxMin.z, 0),"
	"			vec4(0, 0, 0, 1));"
	"    vec4 centre = vec4((boundingBoxMax + boundingBoxMin)/2, 0);"
	"    vec4 V = scale*vec4(vertex-vec3(0.5), 1);"
	"    gl_Position = modelViewProjectionMatrix * (centre+V);"
	"}";
	vs = new QOpenGLShader(QOpenGLShader::Vertex,this);
	vs->compileSourceCode(vs_src);
	cout << "VS log:" << vs->log().toStdString() << endl;
	
	QString fs_src =
	"#version 330 core\n"
	"in vec4 frontColor;"
	"out vec4 fragColor;"
	"void main()"
	"{"
	"    fragColor = frontColor;"
	"}";
	fs = new QOpenGLShader(QOpenGLShader::Fragment,this);
	fs->compileSourceCode(fs_src);
	cout << "FS log:" << fs->log().toStdString() << endl;
	
	program = new QOpenGLShaderProgram(this);
	program->addShader(vs);
	program->addShader(fs);
	program->link();
	cout << "Link log:" << program->log().toStdString() << endl;
	
	created = false;
}

void DrawBoundingBox::postFrame()
{
	program->bind();
	
	//create buffers
	GLWidget& g = *glwidget();
	g.makeCurrent();
	if(!created) {
		created = true;
		GLfloat cube_vertex[] = {
			0,0,0,
			0,0,1,
			0,1,0,
			
			0,0,1,
			0,1,0,
			0,1,1,
			
			1,0,0,
			1,0,1,
			1,1,0,
			
			1,0,1,
			1,1,0,
			1,1,1,
			
			0,0,0,
			0,0,1,
			1,0,0,
			
			0,0,1,
			1,0,0,
			1,0,1,
			
			0,1,0,
			0,1,1,
			1,1,0,
			
			0,1,1,
			1,1,0,
			1,1,1,
			
			0,0,0,
			0,1,0,
			1,0,0,
			
			0,1,0,
			1,0,0,
			1,1,0,
			
			0,0,1,
			0,1,1,
			1,0,1,
			
			0,1,1,
			1,0,1,
			1,1,1
		};
		
		GLfloat cube_colors[] = {
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0,
			1,0,0
		};
		
		//VAO Box
		g.glGenVertexArrays(1, &cubeVAO);
		g.glBindVertexArray(cubeVAO);
		
		//VBOs box
		g.glGenBuffers(1, &vertexVBO);
		g.glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		g.glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex), cube_vertex, GL_STATIC_DRAW);
		g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		g.glEnableVertexAttribArray(0);
		
		g.glGenBuffers(1, &colorVBO);
		g.glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		g.glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
		g.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		g.glEnableVertexAttribArray(2);
		
		g.glBindVertexArray(0);
	}
	
	QMatrix4x4 MVP = camera()->projectionMatrix()*camera()->viewMatrix();
	program->setUniformValue("modelViewProjectionMatrix",MVP);
	
	Scene* scn = scene();
	const vector<Object> &obj = scn->objects();
	for(int i=0; i<(int)obj.size(); ++i) {
		program->setUniformValue("boundingBoxMin",obj[i].boundingBox().min());
		program->setUniformValue("boundingBoxMax",obj[i].boundingBox().max());
		
		g.glBindVertexArray(cubeVAO);
		g.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		g.glDrawArrays(GL_TRIANGLES, 0, 36);
		g.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		g.glBindVertexArray(0);
	}
	
	program->release();
	
	program->release();
}


