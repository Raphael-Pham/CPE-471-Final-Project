#include "Application.h"

void Application::keyCallback(
    GLFWwindow *window, 
    int key, 
    int scancode, 
    int action, 
    int mods)
{
    float speed = 1.3;
    keyToggles[key] = ! keyToggles[key];

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        eyePos.x += -speed * w.x;
        eyePos.z += -speed * w.z;
        lookAtPtDelta.x += -speed * w.x;
        lookAtPtDelta.z += -speed * w.z;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        eyePos.x += -speed * u.x;
        eyePos.z += -speed * u.z;
        lookAtPtDelta.x += -speed * u.x;
        lookAtPtDelta.z += -speed * u.z;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        eyePos.x += speed * u.x;
        eyePos.z += speed * u.z;
        lookAtPtDelta.x += speed * u.x;
        lookAtPtDelta.z += speed * u.z;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        eyePos.x += speed * w.x;
        eyePos.z += speed * w.z;
        lookAtPtDelta.x += speed * w.x;
        lookAtPtDelta.z += speed * w.z;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        gCamH  -= 0.25;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        lightTrans += 5;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        lightTrans -= 5;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        if (material < 5)
            material += 1;
        else
            material = 0;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        goCamera = !goCamera;

        eyePosInit = vec3(-8, 2, 0);
        eyePos = eyePosInit;

        scrollTheta = 0;
        scrollPhi = 0.0;

        lookAtPt.x = cos(scrollPhi) * cos(scrollTheta) + lookAtPtDelta.x + eyePosInit.x;
        lookAtPt.y = sin(scrollPhi) + lookAtPtDelta.y + eyePosInit.y;
        lookAtPt.z = cos(scrollPhi) * cos((PI / 2.0) - scrollTheta) + lookAtPtDelta.z + eyePosInit.z;
        lookAtPtDelta = vec3(0, 0, 0);
    }
}

void Application::mouseCallback(
    GLFWwindow *window, 
    int button, 
    int action, 
    int mods)
{
    double posX, posY;
    if (action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &posX, &posY);
        cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
    }
}

void Application::scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {
    int width, height;
    float x, y, z;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    scrollPhi += 10 * (PI / height) * deltaY;
    if (scrollPhi >= 1.4)
        scrollPhi = 1.4;
    else if (scrollPhi <= -1.4)
        scrollPhi = -1.4;

    scrollTheta += 20 * (PI / width) * deltaX;
}

void Application::resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::initShaders(const std::string& resourceDirectory)
{
    // Blinn-Phong:
    prog = make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", 
                         resourceDirectory + "/simple_frag.glsl");
    prog->init();
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addUniform("MatAmb");
    prog->addUniform("MatDif");
    prog->addUniform("MatSpec");
    prog->addUniform("MatShine");
    prog->addUniform("lightPos1");
    prog->addUniform("lightPos2");
    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");

    // Textures:
    texProg = make_shared<Program>();
    texProg->setVerbose(true);
    texProg->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag0.glsl");
    texProg->init();
    texProg->addUniform("P");
    texProg->addUniform("V");
    texProg->addUniform("M");
    texProg->addUniform("Texture0");
    texProg->addAttribute("vertPos");
    texProg->addAttribute("vertNor");
    texProg->addAttribute("vertTex");

    // Skybox:
    cubeProg = make_shared<Program>();
    cubeProg->setVerbose(true);
    cubeProg->setShaderNames(resourceDirectory + "/cube_vert.glsl", resourceDirectory + "/cube_frag.glsl");
    cubeProg->init();
    cubeProg->addUniform("P");
    cubeProg->addUniform("V");
    cubeProg->addUniform("M");
    cubeProg->addUniform("skybox");
    cubeProg->addAttribute("vertPos");
    cubeProg->addAttribute("vertNor");

     // Particles:
    partProg = make_shared<Program>();
    partProg->setVerbose(true);
    partProg->setShaderNames(
        resourceDirectory + "/particle_vert.glsl",
        resourceDirectory + "/particle_frag.glsl");
    if (!partProg->init())
    {
        std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
        exit(1);
    }
    partProg->addUniform("P");
    partProg->addUniform("M");
    partProg->addUniform("V");
    partProg->addUniform("alphaTexture");
    partProg->addAttribute("vertPos");
    partProg->addAttribute("vertCol");
}
void Application::initTextures(const std::string& resourceDirectory)
{
    groundTex = make_shared<Texture>();
    groundTex->setFilename(resourceDirectory + "/grass.jpg");
    groundTex->init();
    groundTex->setUnit(0);
    groundTex->setWrapModes(GL_REPEAT, GL_REPEAT);

    fireTex = make_shared<Texture>();
    fireTex->setFilename(resourceDirectory + "/alpha.bmp");
    fireTex->init();
    fireTex->setUnit(0);
    fireTex->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}
void Application::init(const std::string& resourceDirectory)
{
    GLSL::checkVersion();

    // Set background color.
    CHECKED_GL_CALL(glClearColor(0.098f, 0.098f, 0.098f, 0.1f));
    // Enable z-buffer test.
    CHECKED_GL_CALL(glEnable(GL_DEPTH_TEST));
    CHECKED_GL_CALL(glEnable(GL_BLEND));
    // CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    CHECKED_GL_CALL(glPointSize(24.0f));

    // Initialize shaders & textures:
    initShaders(resourceDirectory);
    initTextures(resourceDirectory);

    // Set spline path:
    splinepath[0] = Spline(glm::vec3(-8,2,0), glm::vec3(-15,15,0), glm::vec3(0, 30, 30), glm::vec3(-30,50,30), 8);
    splinepath[1] = Spline(glm::vec3(-30,50,30), glm::vec3(-20,30,-20), glm::vec3(-15, 15, 0), glm::vec3(-8, 2, 0), 8);

    // Initialize particle system for campfire:
    thePartSystem = new particleSys(vec3(0, -0.5, 0));
	thePartSystem->gpuSetup();
}

void Application::initCampfire(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    shared_ptr<Shape> firePiece;
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/campfire.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        fireGMin.x = fireGMin.y = fireGMin.z = INT16_MAX;
        fireGMax.x = fireGMax.y = fireGMax.z = INT16_MIN;

        for (int i = 0; i < TOshapes.size(); i++) {
            firePiece = make_shared<Shape>();
            firePiece->createShape(TOshapes[i]);
            firePiece->measure();
            firePiece->init();
            firePieces.push_back(firePiece);

            if (firePiece->min.x < fireGMin.x) fireGMin.x = firePiece->min.x;
            if (firePiece->min.y < fireGMin.y) fireGMin.y = firePiece->min.y;
            if (firePiece->min.z < fireGMin.z) fireGMin.z = firePiece->min.z;
            if (firePiece->max.x > fireGMax.x) fireGMax.x = firePiece->max.x;
            if (firePiece->max.y > fireGMax.y) fireGMax.y = firePiece->max.y;
            if (firePiece->max.z > fireGMax.z) fireGMax.z = firePiece->max.z;
        }
    }
}
void Application::initTent(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    shared_ptr<Shape> tentPiece;
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/tent.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        tentGMin.x = tentGMin.y = tentGMin.z = INT16_MAX;
        tentGMax.x = tentGMax.y = tentGMax.z = INT16_MIN;

        for (int i = 0; i < TOshapes.size(); i++) {
            tentPiece = make_shared<Shape>();
            tentPiece->createShape(TOshapes[i]);
            tentPiece->measure();
            tentPiece->init();
            tentPieces.push_back(tentPiece);

            if (tentPiece->min.x < tentGMin.x) tentGMin.x = tentPiece->min.x;
            if (tentPiece->min.y < tentGMin.y) tentGMin.y = tentPiece->min.y;
            if (tentPiece->min.z < tentGMin.z) tentGMin.z = tentPiece->min.z;
            if (tentPiece->max.x > tentGMax.x) tentGMax.x = tentPiece->max.x;
            if (tentPiece->max.y > tentGMax.y) tentGMax.y = tentPiece->max.y;
            if (tentPiece->max.z > tentGMax.z) tentGMax.z = tentPiece->max.z;
        }
    }
}
void Application::initTable(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    shared_ptr<Shape> tablePiece;
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/table.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        tableGMin.x = tableGMin.y = tableGMin.z = INT16_MAX;
        tableGMax.x = tableGMax.y = tableGMax.z = INT16_MIN;

        for (int i = 0; i < TOshapes.size(); i++) {
            tablePiece = make_shared<Shape>();
            tablePiece->createShape(TOshapes[i]);
            tablePiece->measure();
            tablePiece->init();
            tablePieces.push_back(tablePiece);

            if (tablePiece->min.x < tableGMin.x) tableGMin.x = tablePiece->min.x;
            if (tablePiece->min.y < tableGMin.y) tableGMin.y = tablePiece->min.y;
            if (tablePiece->min.z < tableGMin.z) tableGMin.z = tablePiece->min.z;
            if (tablePiece->max.x > tableGMax.x) tableGMax.x = tablePiece->max.x;
            if (tablePiece->max.y > tableGMax.y) tableGMax.y = tablePiece->max.y;
            if (tablePiece->max.z > tableGMax.z) tableGMax.z = tablePiece->max.z;
        }
    }
}
void Application::initLog(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/log.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        log = make_shared<Shape>();
        log->createShape(TOshapes[0]);
        log->measure();
        log->init();
    }
}
void Application::initTrees(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    for (int i = 0; i < 50; i++) {
        shared_ptr<Shape> treePiece;
        bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/tree.obj").c_str());
        if (!rc) {
            cerr << errStr << endl;
        } else {
            treeGMin.x = treeGMin.y = treeGMin.z = INT16_MAX;
            treeGMax.x = treeGMax.y = treeGMax.z = INT16_MIN;

            for (int i = 0; i < TOshapes.size(); i++) {
                treePiece = make_shared<Shape>();
                treePiece->createShape(TOshapes[i]);
                treePiece->measure();
                treePiece->init();
                treePieces.push_back(treePiece);

                if (treePiece->min.x < treeGMin.x) treeGMin.x = treePiece->min.x;
                if (treePiece->min.y < treeGMin.y) treeGMin.y = treePiece->min.y;
                if (treePiece->min.z < treeGMin.z) treeGMin.z = treePiece->min.z;
                if (treePiece->max.x > treeGMax.x) treeGMax.x = treePiece->max.x;
                if (treePiece->max.y > treeGMax.y) treeGMax.y = treePiece->max.y;
                if (treePiece->max.z > treeGMax.z) treeGMax.z = treePiece->max.z;
            }
            trees.push_back(treePieces);
            if (i < 49)
                treePieces.clear();
        }
    }
}
void Application::initSkybox(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/cube.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        cube = make_shared<Shape>();
        cube->createShape(TOshapes[0]);
        cube->measure();
        cube->init();
    }
}
void Application::initDummy(
    const std::string& resourceDirectory, 
    vector<tinyobj::material_t> objMaterials,
    vector<tinyobj::shape_t> TOshapes,
    string errStr)
{
    shared_ptr<Shape> dummyShape;
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/dummy.obj").c_str());
    if (!rc) {
        cerr << errStr << endl;
    } else {
        dummyGMin.x = dummyGMin.y = dummyGMin.z = INT16_MAX;
        dummyGMax.x = dummyGMax.y = dummyGMax.z = INT16_MIN;

        for (int i = 0; i < TOshapes.size(); i++) {
            dummyShape = make_shared<Shape>();
            dummyShape->createShape(TOshapes[i]);
            dummyShape->measure();
            dummyShape->init();
            dummyShapes.push_back(dummyShape);

            if (dummyShape->min.x < dummyGMin.x) dummyGMin.x = dummyShape->min.x;
            if (dummyShape->min.y < dummyGMin.y) dummyGMin.y = dummyShape->min.y;
            if (dummyShape->min.z < dummyGMin.z) dummyGMin.z = dummyShape->min.z;
            if (dummyShape->max.x > dummyGMax.x) dummyGMax.x = dummyShape->max.x;
            if (dummyShape->max.y > dummyGMax.y) dummyGMax.y = dummyShape->max.y;
            if (dummyShape->max.z > dummyGMax.z) dummyGMax.z = dummyShape->max.z;
        }
    }
}
void Application::initGeom(const std::string& resourceDirectory)
{
    vector<tinyobj::material_t> objMaterials;
    vector<tinyobj::shape_t> TOshapes;
    string errStr;

    initCampfire(resourceDirectory, objMaterials, TOshapes, errStr);
    initTent(resourceDirectory, objMaterials, TOshapes, errStr);
    initTable(resourceDirectory, objMaterials, TOshapes, errStr);
    initLog(resourceDirectory, objMaterials, TOshapes, errStr);
    initTrees(resourceDirectory, objMaterials, TOshapes, errStr);
    initSkybox(resourceDirectory, objMaterials, TOshapes, errStr);
    initDummy(resourceDirectory, objMaterials, TOshapes, errStr);
    initGround();
}

void Application::initGround() 
{
    float g_groundSize = 100;
    float g_groundY = -0.25;

    // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
    float GrndPos[] = {
        -g_groundSize, g_groundY, -g_groundSize,
        -g_groundSize, g_groundY,  g_groundSize,
        g_groundSize, g_groundY,  g_groundSize,
        g_groundSize, g_groundY, -g_groundSize
    };

    float GrndNorm[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0
    };

    static GLfloat GrndTex[] = {
        0, 0, // back
        0, 10,
        10, 10,
        10, 0 };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    //generate the ground VAO
    glGenVertexArrays(1, &GroundVertexArrayID);
    glBindVertexArray(GroundVertexArrayID);

    g_GiboLen = 6;
    glGenBuffers(1, &GrndBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

    glGenBuffers(1, &GrndNorBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

    glGenBuffers(1, &GrndTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void Application::drawGround(shared_ptr<Program> curS) 
{
    curS->bind();
    glBindVertexArray(GroundVertexArrayID);
    groundTex->bind(curS->getUniform("Texture0"));
    
    // Draw the ground plane :
    mat4 ctm = setModel(vec3(0, -1, 0), 0, 0, 0, 1);
    glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    curS->unbind();
}

unsigned int Application::createSky(string dir, vector<string> faces) 
{   
    unsigned int textureID;  
    glGenTextures(1, &textureID);  
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);  
    int width, height, nrChannels;   
    stbi_set_flip_vertically_on_load(false);

    for (GLuint i = 0; i < faces.size(); i++) 
    {     
        unsigned char *data = stbi_load((dir+faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data) 
        {         
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  
        } else {    
            cout << "failed to load: " << (dir+faces[i]).c_str() << endl;  
        }   
    }   
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);     
    cout << " creating cube map any errors : " << glGetError() << endl;   
    
    return textureID; 
} 

//helper function to pass material data to the GPU
void Application::SetMaterial(shared_ptr<Program> curS, int i) 
{
    switch (i) {
        case 0:
            glUniform3f(curS->getUniform("MatAmb"), 0.096, 0.046, 0.095);
            glUniform3f(curS->getUniform("MatDif"), 0.96, 0.46, 0.95);
            glUniform3f(curS->getUniform("MatSpec"), 0.45, 0.23, 0.45);
            glUniform1f(curS->getUniform("MatShine"), 80.0);
        break;
        case 1:
            glUniform3f(curS->getUniform("MatAmb"), 0.063, 0.038, 0.1);
            glUniform3f(curS->getUniform("MatDif"), 0.63, 0.38, 1.0);
            glUniform3f(curS->getUniform("MatSpec"), 0.3, 0.2, 0.5);
            glUniform1f(curS->getUniform("MatShine"), 8.0);
        break;
        case 2:
            glUniform3f(curS->getUniform("MatAmb"), 0.004, 0.05, 0.09);
            glUniform3f(curS->getUniform("MatDif"), 0.04, 0.5, 0.9);
            glUniform3f(curS->getUniform("MatSpec"), 0.02, 0.25, 0.45);
            glUniform1f(curS->getUniform("MatShine"), 27.9);
        break;
        case 3:
            glUniform3f(curS->getUniform("MatAmb"), 0.02, 0.0, 0.0);
            glUniform3f(curS->getUniform("MatDif"), 0.2, 0.0, 0.0);
            glUniform3f(curS->getUniform("MatSpec"), 0.1, 0.0, 0.0);
            glUniform1f(curS->getUniform("MatShine"), 16.0);
        break;
        case 4:
            glUniform3f(curS->getUniform("MatAmb"), 0.0753, 0.0753, 0.0753);
            glUniform3f(curS->getUniform("MatDif"), 0.753, 0.753, 0.753);
            glUniform3f(curS->getUniform("MatSpec"), 0.376, 0.376, 0.376);
            glUniform1f(curS->getUniform("MatShine"), 32.0);
        break;
        case 5:
            glUniform3f(curS->getUniform("MatAmb"), 0.0361, 0.0251, 0.02);
            glUniform3f(curS->getUniform("MatDif"), 0.361, 0.251, 0.2);
            glUniform3f(curS->getUniform("MatSpec"), 0.18, 0.125, 0.1);
            glUniform1f(curS->getUniform("MatShine"), 60.0);
        break;
        case 6:
            glUniform3f(curS->getUniform("MatAmb"), 0.0, 0.02, 0.0);
            glUniform3f(curS->getUniform("MatDif"), 0.0, 0.2, 0.0);
            glUniform3f(curS->getUniform("MatSpec"), 0.0, 0.1, 0.0);
            glUniform1f(curS->getUniform("MatShine"), 16.0);
        break;
    }
}

/* helper function to set model transforms */
mat4 Application::setModel(
    vec3 trans, 
    float rotX, 
    float rotY, 
    float rotZ, 
    float sc) 
{
    mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
    mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, vec3(1, 0, 0));
    mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, vec3(0, 1, 0));
    mat4 RotZ = glm::rotate( glm::mat4(1.0f), rotZ, vec3(0, 0, 1));
    mat4 ScaleS = glm::scale(glm::mat4(1.0f), vec3(sc));
    return Trans*RotX*RotY*RotZ*ScaleS;
}

void Application::setModel(
    std::shared_ptr<Program> prog, 
    std::shared_ptr<MatrixStack>M) 
{
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
}

vector<float> Application::getMidPoint(float maxX, float minX, 
                          float maxY, float minY, 
                          float maxZ, float minZ) {
    vector<float> ret;

    ret.push_back((maxX + minX) / 2);
    ret.push_back((maxY + minY) / 2);
    ret.push_back((maxZ + minZ) / 2);

    return ret;
}

void Application::updateUsingCameraPath(float frametime){
    if (goCamera) {
        if(!splinepath[0].isDone()){
            splinepath[0].update(frametime);
            eyePos = splinepath[0].getPosition();
        } else {
            splinepath[1].update(frametime);
            eyePos = splinepath[1].getPosition();
        }
    }
    if (splinepath[1].getPosition() == vec3(-8, 2, 0)) {
        goCamera = false;
    }
}

void Application::render(float frametime) 
{
    // Get current frame buffer size.
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    glViewport(0, 0, width, height);

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Use the matrix stack for Lab 6
    float aspect = width/(float)height;

    // Create the matrix stacks - please leave these alone for now
    auto Projection = make_shared<MatrixStack>();
    auto View = make_shared<MatrixStack>();
    auto Model = make_shared<MatrixStack>();

    // Update the camera position
	updateUsingCameraPath(frametime);

    // Apply perspective projection.
    Projection->pushMatrix();
    Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

    // Update lookAtPt
    lookAtPt.x = cos(scrollPhi) * cos(scrollTheta) + lookAtPtDelta.x + eyePosInit.x;
    lookAtPt.y = sin(scrollPhi) + lookAtPtDelta.y + eyePosInit.y;
    lookAtPt.z = cos(scrollPhi) * cos((PI / 2.0) - scrollTheta) + lookAtPtDelta.z + eyePosInit.z;

    // Update basis vectors
    vec3 gaze = normalize(lookAtPt - eyePos);
    w = -gaze / length(gaze);
    u = cross(vec3(0, 1, 0), w);
    v = cross(w, u);

    // View is global translation along negative z for now
    View->pushMatrix();
    View->loadIdentity();
    View->lookAt(eyePos, lookAtPt, vec3(0, 1, 0));

    // Draw the skybox
    //to draw the sky box bind the right shader 
    Model->pushMatrix();
        cubeProg->bind(); 
        //set the projection matrix - can use the same one 
        glUniformMatrix4fv(cubeProg->getUniform("P"), 1, GL_FALSE, 
        value_ptr(Projection->topMatrix())); 
        //set the depth function to always draw the box! 
        glDepthFunc(GL_LEQUAL); 
        //set up view matrix to include your view transforms 
        //(your code likely will be different depending 
        glUniformMatrix4fv(cubeProg->getUniform("V"), 1,
        GL_FALSE,value_ptr(View->topMatrix()) ); 
        //set and send model transforms - likely want a biggerdel->popMatrix();
        Model->rotate(glfwGetTime() * 0.015, vec3(0, 1, 0));
        Model->scale(110);
        glUniformMatrix4fv(cubeProg->getUniform("M"), 1, 
        GL_FALSE,value_ptr(Model->topMatrix())); 
        //bind the sphere map texture 
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture); 
        //draw the actual cube 
        cube->draw(cubeProg); 
        //set the depth test back to normal! 
        glDepthFunc(GL_LESS); 
        //unbind the shader for the skybox 
        cubeProg->unbind();
    Model->popMatrix();

    // Draw the scene
    prog->bind();
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
    glUniform3f(prog->getUniform("lightPos1"), -2.0 + lightTrans, 2.0, 2.0);
    glUniform3f(prog->getUniform("lightPos2"), 0.0 + lightTrans, -0.25, 5.0);

    float midX, midY, midZ;
    vector<float> midPoint;
    mat4 ctm, trans, rotX, rotY, rotZ, scale;
    mat4 trans1, trans2;

    // Draw campfire:
    SetMaterial(prog, 3);
    midPoint = getMidPoint(fireGMax.x, fireGMin.x,
                           fireGMax.y, fireGMin.y,
                           fireGMax.z, fireGMin.z);
    trans = setModel(vec3(-1 * midPoint.at(0), -1 * midPoint.at(1), -1 * midPoint.at(2)),
                      0, 0, 0, 1);
    scale = setModel(vec3(0, 0, 0), 0, 0, 0, 1);
    trans2 = setModel(vec3(0, -0.25, 0), 0, 0, 0, 1);
    ctm = trans2 * scale * trans;
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
    for (int i = 0; i < firePieces.size(); i++)
        firePieces.at(i)->draw(prog);

    // Draw tent:
    SetMaterial(prog, material);
    midPoint = getMidPoint(tentGMax.x, tentGMin.x,
                           tentGMax.y, tentGMin.y,
                           tentGMax.z, tentGMin.z);
    trans1 = setModel(vec3(-1 * midPoint.at(0), -1 * midPoint.at(1), -1 * midPoint.at(2)),
                      0, 0, 0, 1);
    scale = setModel(vec3(0, 0, 0), 0, 0, 0, 2);
    rotY = setModel(vec3(0, 0, 0), 0, PI / 2, 0, 1);
    trans2 = setModel(vec3(0, -0.25, 11), 0, 0, 0, 1);
    ctm = trans2 * rotY * scale * trans1;
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
    for (int i = 0; i < tentPieces.size(); i++)
        tentPieces.at(i)->draw(prog);

    // Draw table:
    SetMaterial(prog, 5);
    midPoint = getMidPoint(tableGMax.x, tableGMin.x,
                           tableGMax.y, tableGMin.y,
                           tableGMax.z, tableGMin.z);
    trans1 = setModel(vec3(-1 * midPoint.at(0), -1 * midPoint.at(1), -1 * midPoint.at(2)),
                      0, 0, 0, 1);
    rotX = setModel(vec3(0, 0, 0), -PI / 2, 0, 0, 1);
    scale = setModel(vec3(0, 0, 0), 0, 0, 0, 0.05);
    trans2 = setModel(vec3(0, -0.25, -5), 0, 0, 0, 1);
    ctm = trans2 * rotX * scale * trans1;
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
    for (int i = 0; i < tablePieces.size(); i++)
        tablePieces.at(i)->draw(prog);

    // Draw log:
    SetMaterial(prog, 5);
    rotX = setModel(vec3(0, 0, 0), PI, 0, 0, 1);
    scale = setModel(vec3(0, 0, 0), 0, 0, 0, 0.67);
    rotY = setModel(vec3(0, 0, 0), 0, PI / 2, 0, 1);
    trans = setModel(vec3(5, -0.25, 0), 0, 0, 0, 1);
    ctm = trans * rotX * rotY * scale;
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
    log->draw(prog);

    // Draw trees:
    SetMaterial(prog, 6);
    midPoint = getMidPoint(treeGMax.x, treeGMin.x,
                           treeGMax.y, treeGMin.y,
                           treeGMax.z, treeGMin.z);
    int treesMade = 0;
    while (treesMade < trees.size()) {
        float xRand = randTrans[treesMade][0];
        float zRand = randTrans[treesMade][1];

        trans1 = setModel(vec3(-1 * midPoint.at(0), -1 * midPoint.at(1), -1 * midPoint.at(2)),
                    0, 0, 0, 1);
        scale = setModel(vec3(0, 0, 0), 0, 0, 0, randScale.at(treesMade));
        trans2 = setModel(vec3(xRand, 5, zRand), 0, 0, 0, 1);
        ctm = trans2 * scale * trans1;
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
        vector<shared_ptr<Shape>> pieces = trees.at(treesMade);
        for (int j = 0; j < pieces.size(); j++){
            pieces.at(j)->draw(prog);
        }
        treesMade++;
    }

    // Draw dummy:
    SetMaterial(prog, 4);
    scale = setModel(vec3(0, 0, 0), 0, 0, 0, 0.02);
    rotX = setModel(vec3(0, 0, 0), -PI / 2, 0, 0, 1);
    ctm = rotX * scale;
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));

    
    Model->pushMatrix();
        Model->loadIdentity();
        // Entire dummy transformations:
        Model->rotate(-PI/2, vec3(1, 0, 0));
        Model->scale(0.018);

        Model->pushMatrix();
            Model->pushMatrix();
                // Draw right arm:
                Model->translate(vec3(2.33, -19.81, 137.60));
                Model->rotate(rArmTheta, vec3(1, 0, 0));
                Model->translate(vec3(-2.33, 19.81, -137.60));
                setModel(prog, Model);
                for (int i = 15; i < 21; i++)
                    dummyShapes.at(i)->draw(prog);
            Model->popMatrix();

            Model->pushMatrix();
                // Draw left upper arm:
                Model->translate(vec3(2.33, 19.81, 137.60));
                Model->rotate(lShoulderForwardTheta, vec3(0, 1, 0));
                Model->rotate(lShoulderDownTheta, vec3(1, 0, 0));
                Model->translate(vec3(-2.33, -19.81, -137.60));
                setModel(prog, Model);
                for (int i = 21; i < 23; i++)
                    dummyShapes.at(i)->draw(prog);
                Model->pushMatrix();
                    // Draw left lower arm:
                    Model->translate(vec3(2.33, 47.63, 137.60));
                    Model->rotate(lElbowUpTheta, vec3(0, 0, 1));
                    Model->rotate(lElbowInTheta, vec3(1, 0, 0));
                    Model->translate(vec3(-2.33, -47.63, -137.60));
                    setModel(prog, Model);
                    for (int i = 23; i < 25; i++)
                        dummyShapes.at(i)->draw(prog);
                    Model->pushMatrix();
                        // Draw left hand (bent wrist):
                        Model->translate(vec3(2.33, 73.06, 137.60));
                        Model->rotate(lWristDownTheta, vec3(1, 0, 0));
                        Model->rotate(lWristTwistTheta, vec3(0, 1, 0));
                        Model->translate(vec3(-2.33, -73.06, -137.60));
                        setModel(prog, Model);
                        for (int i = 25; i < 27; i++)
                            dummyShapes.at(i)->draw(prog);
                    Model->popMatrix();
                Model->popMatrix();
            Model->popMatrix();
            // Draw torso, belly, neck, head:
            setModel(prog, Model);
            for (int i = 13; i < 29; i++) {
                if (i < 15 || i > 26)
                    dummyShapes.at(i)->draw(prog);
            }
        Model->popMatrix();
    Model->popMatrix();

    prog->unbind();

    midPoint = getMidPoint(dummyShapes.at(25)->max.x,
                            dummyShapes.at(25)->min.x,
                            dummyShapes.at(25)->max.y,
                            dummyShapes.at(25)->min.y,
                            dummyShapes.at(25)->max.z,
                            dummyShapes.at(25)->min.z);
    cout << "x: " << midPoint[0] << endl;
    cout << "y: " << midPoint[1] << endl;
    cout << "z: " << midPoint[2] << endl;
    

    // Dummy animation data:
    

    //switch shaders to the texture mapping shader and draw the ground
    texProg->bind();
    glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
    glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
    glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
            
    drawGround(texProg);
    texProg->unbind();

    thePartSystem->setCamera(View->topMatrix());

    // Draw campfire particles
    CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    partProg->bind();
    fireTex->bind(partProg->getUniform("alphaTexture"));
    CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
    CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
    CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
    
    thePartSystem->drawMe(partProg);
    thePartSystem->update();

    partProg->unbind();
    CHECKED_GL_CALL(glBlendFunc(GL_ONE, GL_ZERO));

    // Pop matrix stacks.
    Projection->popMatrix();
    View->popMatrix();
}

vector<float> Application::genRandScale()
{
    vector<float> res;
    for (int i = 0; i < 50; i++) {
        res.push_back(rand() % 10 + 3);
    }
    return res;
}
vector<vector<float>> Application::genRandTrans()
{
    vector<vector<float>> res;
    float xRand, zRand;
    int i = 0;

    while (i < 50) {
        xRand = rand() % 100 - 50;
        zRand = rand() % 100 - 50;
        while (xRand < 25 && xRand > -25 && zRand < 25 && zRand > -25) {
            xRand = rand() % 100 - 50;
            zRand = rand() % 100 - 50;
        }
        vector<float> pair{xRand, zRand};
        res.push_back(pair);
        i++;
    }
    return res;
}
