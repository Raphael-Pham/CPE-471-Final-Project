#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <glad/glad.h>
#include <chrono>
#include <set>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Texture.h"
#include "stb_image.h"
#include "Bezier.h"
#include "Spline.h"
#include "particleSys.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.1415926585

using namespace glm;

class Application : public EventCallbacks
{
    public:
        // Shaders:
        // Blinn-Phong
        std::shared_ptr<Program> prog;
        // Textures
        std::shared_ptr<Program> texProg;
        // Skybox
        std::shared_ptr<Program> cubeProg;
        // Particles
	    std::shared_ptr<Program> partProg;

        // The particle system:
	    particleSys *thePartSystem;

        // Geometry:
        // Fire
        vector<std::shared_ptr<Shape>> firePieces;
        vec3 fireGMin, fireGMax;
        // Tent
        vector<shared_ptr<Shape>> tentPieces;
        vec3 tentGMin, tentGMax;
        // Table
        vector<shared_ptr<Shape>> tablePieces;
        vec3 tableGMin, tableGMax;
        // Dummy
        vector<shared_ptr<Shape>> dummyShapes;
        vec3 dummyGMin, dummyGMax;
        // Tree
        vector<shared_ptr<Shape>> treePieces;
        vec3 treeGMin, treeGMax;
        vector<vector<shared_ptr<Shape>>> trees;
        // Log
        shared_ptr<Shape> log;
        // Lantern
        vector<shared_ptr<Shape>> lanternPieces;
        vec3 lanternGMin, lanternGMax;
        // Watchtower
        vector<shared_ptr<Shape>> towerPieces;
        vec3 towerGMin, towerGMax;

        // Random tree generation arrays:
        vector<float> randScale = genRandScale();
        vector<vector<float>> randTrans = genRandTrans();

        // Ground plane data:
        GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj, GroundVertexArrayID;
        int g_GiboLen;

        // Textures:
        // Ground
        shared_ptr<Texture> groundTex;
        // Fire
	    shared_ptr<Texture> fireTex;

        // Particle variables:
        float t = 0.0f; 
        float h = 0.01f;

        // Global data:
        vec3 gMin;
        float gRot = 0;
        float gCamH = -10;

        // Animation data:
        int material = 4;
        float lightTrans = 0;
        float gTrans = -3;
        float sTheta = 0;
        float eTheta = 0;
        float hTheta = 0;

        // Dummy animation data:
        float rArmDownTheta = 0;
        float rArmForwardTheta = 0;
        float lWristDownTheta = 0;
        float lWristTwistTheta = 0;
        float lShoulderDownTheta = 0;
        float lShoulderForwardTheta = 0;
        float lElbowInTheta = 0;
        float lElbowUpTheta = 0;
        float lKneeTheta = 0;
        float lPelvisTheta = 0;
        float rKneeTheta = 0;
        float rPelvisTheta = 0;
        float lanternSpin = 0;

        // Camera scrolling variables:
        vec3 eyePosInit = vec3(-8, 1.5, 0);
        vec3 eyePos = eyePosInit;
        vec3 lookAtPt = vec3(0, 0, 0);
        vec3 lookAtPtDelta = vec3(0, 0, 0);
        float scrollTheta = 0;
        float scrollPhi = 0.0;
        bool firstPerson = true;

        // Basis vectors:
        vec3 u = vec3(0, 0, 0);
        vec3 v = vec3(0, 0, 0);
        vec3 w = vec3(0, 0, 0);

        // Skybox data:
        unsigned int cubeMapTexture;
        vector<std::string> faces {
            "/galaxy/galaxy+X.tga",
            "/galaxy/galaxy-X.tga",
            "/galaxy/galaxy+Y.tga",
            "/galaxy/galaxy-Y.tga",
            "/galaxy/galaxy+Z.tga",
            "/galaxy/galaxy-Z.tga"
        };
        shared_ptr<Shape> cube;

        // Spline path vars:
        Spline splinepath[2];
	    bool goCamera = false;

        // Movement data:
        bool moving, wPress, sPress, aPress, dPress = false;
        char lastPressed;
        vec3 dummyPos = vec3(-8.5, -1, 0);
        float dummyTurn = 0;

        // Collision detection:
        vector<pair<float, float>> bounds;

        // Misc:
        WindowManager *windowManager = nullptr;
        bool keyToggles[256] = { false };
        
        // Methods:
        void keyCallback(GLFWwindow *, int, int, int, int);
        void mouseCallback(GLFWwindow *, int, int, int);
        void scrollCallback(GLFWwindow *, double, double);
        void resizeCallback(GLFWwindow *, int, int);

        void initShaders(const std::string&);
        void initTextures(const std::string&);
        void init(const std::string&);
        void initCampfire(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initTent(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initTable(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initLog(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initTrees(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initSkybox(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initDummy(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initLantern(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initTower(const std::string&, vector<tinyobj::material_t>, vector<tinyobj::shape_t>, string);
        void initGeom(const std::string&);
        void initGround();

        void drawGround(shared_ptr<Program>);
        unsigned int createSky(string, vector<string>);
        void SetMaterial(shared_ptr<Program>, int);
  	    mat4 setModel(vec3, float, float, float, float);
        void setModel(std::shared_ptr<Program>, std::shared_ptr<MatrixStack>);
        vector<float> getMidPoint(float, float, 
                          float, float, 
                          float, float);
        void updateUsingCameraPath(float);
        float renderSetup();
        void updateCam();
        void drawSkybox(shared_ptr<MatrixStack>, shared_ptr<MatrixStack>, shared_ptr<MatrixStack>);
        void drawLights(shared_ptr<Program>);
        void drawCampfire();
        void drawTent();
        void drawTable();
        void drawTower();
        void drawLog();
        void drawTrees();
        float distance(float, float, float, float);
        bool checkCollision();
        void fixPosition();
        bool checkTower();
        void moveStairs();
        void drawDummy(shared_ptr<MatrixStack>);
        void renderGround(shared_ptr<MatrixStack>, shared_ptr<MatrixStack>,
                          shared_ptr<MatrixStack>, shared_ptr<Program>);
        void drawCampfireParticles(shared_ptr<MatrixStack>, shared_ptr<MatrixStack>,
                                   shared_ptr<MatrixStack>);
        void render(float);
        vector<float> genRandScale();
        vector<vector<float>> genRandTrans();
        void movePrimary();
};

#endif