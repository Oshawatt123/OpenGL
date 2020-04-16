#include "Shader.h"

std::string Shader::readFile(const std::string &filePath)
{
	std::string fileContent;
	std::ifstream file;
	file.open(filePath);

	if (!file.is_open())
	{
		std::cerr << "Could not open file " << filePath << " . File does not exist \n";
		return "";
	}

	std::string line = "";
	while (!file.eof())
	{
		std::getline(file, line);
		fileContent.append(line + "\n");
	}

	file.close();
	return fileContent;
}

Shader::Shader(const std::string vertFilepath, const std::string fragFilepath, Camera& _cam)
{

	GLint result = GL_FALSE;
	int logLength;

	std::string vertShaderText = readFile(vertFilepath);
	std::string fragShaderText = readFile(fragFilepath);

	const char* vertShaderSrc = vertShaderText.c_str();
	const char* fragShaderSrc = fragShaderText.c_str();

	std::cout << vertShaderSrc << std::endl;


	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// check shader w/ function
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
	
	// create frag shader
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// check shader w/ function
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	std::cout << &fragShaderError[0] << std::endl;

	// create program
	program = glCreateProgram();
	// attach the shaders
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	// assign uniform locations
	uniformLocation[MODEL_U]			= glGetUniformLocation(program, "model");
	uniformLocation[VIEW_U]				= glGetUniformLocation(program, "view");
	uniformLocation[PROJECTION_U]		= glGetUniformLocation(program, "projection");

	uniformLocation[FRAG_CAMERAPOS_U]	= glGetUniformLocation(program, "FragCamPos");
	uniformLocation[FRAG_LIGHTCOLOR_U]	= glGetUniformLocation(program, "FragLightColor");
	uniformLocation[FRAG_LIGHTPOS_U]	= glGetUniformLocation(program, "FragLightPos");

	uniformLocation[TIME_U] = glGetUniformLocation(program, "u_time");


	// assign camera reference
	cam = &_cam;

	// check program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	// delete shaders
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	shaderFile.close();
}

Shader::~Shader()
{
	// detach the shaders
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	// delete the shaders and program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(program);
}

void Shader::Bind()
{
	glUseProgram(program);
}

void Shader::Update(Transform* transform, LightBase& light)
{
	glUniformMatrix4fv(uniformLocation[MODEL_U], 1, GL_FALSE, &transform->GetModel()[0][0]);
	glUniformMatrix4fv(uniformLocation[PROJECTION_U], 1, GL_FALSE, &cam->GeneratePerspectiveProjectionMatrix()[0][0]);
	glUniformMatrix4fv(uniformLocation[VIEW_U], 1, GL_FALSE, &cam->GenerateViewMatrix()[0][0]);

	glUniform3f(uniformLocation[FRAG_CAMERAPOS_U], cam->m_transform.getPos().x,
		cam->m_transform.getPos().y,
		cam->m_transform.getPos().z);

	glUniform3f(uniformLocation[FRAG_LIGHTPOS_U], light.getTransform()->getPos().x,
		light.getTransform()->getPos().y,
		light.getTransform()->getPos().z);

	std::cout << "X: " << light.getTransform()->getPos().x << " Y: " << light.getTransform()->getPos().y << " Z: " << light.getTransform()->getPos().z << std::endl;

	glUniform3f(uniformLocation[FRAG_LIGHTCOLOR_U], light.getColor().x,
		light.getColor().y,
		light.getColor().z);

	glUniform1f(uniformLocation[TIME_U], (float)Time::Instance()->GetTimeSinceStart());
}