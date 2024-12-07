#define GLM_ENABLE_EXPERIMENTAL
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>

#include "render/opengl_utils.h"
#include "render/geometry_primitives.h"
#include "render/texture.h"
#include "render/texture_cube.h"
#include "render/model.h"
#include "render/mesh.h"
#include "render/scene.h"
#include "render/light.h"

#include "simulation/fluid.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, DirectionalLight* sun);

bool isWindowed = true;
bool isKeyboardDone[1024] = { 0 };

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;
const float planeSize = 15.f;
const float sphereRadius = 0.7f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float cameraFar = 100.0f;
float cameraNear = 0.1f;
bool firstMouse = true;

std::vector<float> shadowCascadeLevels{ cameraFar / 50.0f, cameraFar / 20.0f, cameraFar / 8.0f, cameraFar / 3.0f };

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool useNormalMap = true;
bool useShadow = true;
bool useLighting = true;
bool usePCF = true;
bool useCSM = true;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // build and compile our shader program
    // ------------------------------------
    Shader lightingShader("../shaders/shader_lighting.vs", "../shaders/shader_lighting.fs");
    Shader depthRenderShader("../shaders/shader_depth.vs", "../shaders/shader_depth.fs");
    Shader fluidSurfaceShader("../shaders/shader_surface.vs", "../shaders/shader_surface.fs");
    Shader skyboxShader("../shaders/shader_skybox.vs", "../shaders/shader_skybox.fs");

    // define models
    Model brickCubeModel = Model("../resources/brickcube/brickcube.obj");
    brickCubeModel.diffuse = new Texture("../resources/brickcube/brickcube_d.png");
    brickCubeModel.specular = new Texture("../resources/brickcube/brickcube_s.png");
    brickCubeModel.normal = new Texture("../resources/brickcube/brickcube_n.png");
    
    Model grassGroundModel = Model("../resources/plane.obj");
    grassGroundModel.diffuse = new Texture("../resources/grass_ground.jpg");
    grassGroundModel.ignoreShadow = true;

    // Add entities to scene.
    // you can change the position/orientation.
    Scene scene;
    scene.addEntity(new Entity(&brickCubeModel, glm::mat4(1.0)));

    glm::mat4 planeWorldTransform = glm::mat4(1.0f);
    planeWorldTransform = glm::scale(planeWorldTransform, glm::vec3(planeSize));
    planeWorldTransform = glm::translate(glm::vec3(0.0f, -5.0f, 0.0f)) * planeWorldTransform;
    scene.addEntity(new Entity(&grassGroundModel, planeWorldTransform));

    // skybox
    std::vector<std::string> faces
    {
        "../resources/skybox/right.jpg",
        "../resources/skybox/left.jpg",
        "../resources/skybox/top.jpg",
        "../resources/skybox/bottom.jpg",
        "../resources/skybox/front.jpg",
        "../resources/skybox/back.jpg"
    };
    
    CubemapTexture skyboxTexture = CubemapTexture(faces);
    unsigned int VAOskybox, VBOskybox;
    getPositionVAO(skybox_positions, sizeof(skybox_positions), VAOskybox, VBOskybox);
    
    // depth map
    DepthMapTexture depth(SCR_WIDTH, SCR_HEIGHT);
    unsigned int VAOquad, VBOquad;
    getPositionTexVAO2D(quad_vertices, sizeof(quad_vertices), VAOquad ,VBOquad);
    
    // fluid
    Fluid fluid;
    
    // set uniform values
    lightingShader.use();
    lightingShader.setInt("material.diffuseSampler", 0);
    lightingShader.setInt("material.specularSampler", 1);
    lightingShader.setInt("material.normalSampler", 2);
    lightingShader.setFloat("material.shininess", 64.f);    // set shininess to constant value.

    skyboxShader.use();
    skyboxShader.setInt("skyboxTexture", 0);

    fluidSurfaceShader.use();
    fluidSurfaceShader.setInt("screenTexture", 0);
    
    DirectionalLight sun(30.0f, 30.0f, glm::vec3(0.8f));
    
    
    float oldTime = 0;
    while (!glfwWindowShouldClose(window))// render loop
    {
        float currentTime = glfwGetTime();
        float dt = currentTime - oldTime;
        deltaTime = dt;
        oldTime = currentTime;

        // input
        processInput(window, &sun);
        
        // calculate matrices
        
        glm::mat4 lightProjection = sun.getProjectionMatrix();
        glm::mat4 lightView = sun.getViewMatrix(camera.Position);
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, cameraNear, cameraFar);
        glm::mat4 view = camera.GetViewMatrix();
        
        // clear
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // use lighting Shader
        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("light.dir", sun.lightDir);
        lightingShader.setVec3("light.color", sun.lightColor);
        
        // render objects in the scene
            // framebuffer : default frame buffer(0)
            // shader : shader_lighting.fs/vs
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for (auto const& me : scene.entities) {
            Model* model = me.first;
            model->bind();
            lightingShader.setFloat("useNormalMap", useNormalMap && model->normal ? 1.0f : 0.0f);
            lightingShader.setFloat("useSpecularMap", model->specular ? 1.0f : 0.0f);
            vector<Entity*> entities = me.second;
            for (auto const& e: entities) {
                lightingShader.setMat4("world", e->getModelMatrix());
                glDrawElements(GL_TRIANGLES, model->mesh.indices.size(), GL_UNSIGNED_INT, 0);
            }
        }
        
        // use depth render Shader
        depthRenderShader.use();
        depthRenderShader.setMat4("view", view);
        depthRenderShader.setMat4("projection", projection);
        depthRenderShader.setInt("screenHeight", SCR_HEIGHT);
        depthRenderShader.setFloat("fovy", glm::radians(camera.Zoom));
        depthRenderShader.setFloat("sphereRadius", sphereRadius);
        depthRenderShader.setVec3("light.dir", sun.lightDir);
        depthRenderShader.setVec3("light.color", sun.lightColor);
        fluid.update(deltaTime);
        
        // render depth image
        glBindFramebuffer(GL_FRAMEBUFFER, depth.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
        glBindVertexArray(fluid.VAO);
        glDrawArrays(GL_POINTS, 0, fluid.particles.size());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // use fluid surface render Shader
        fluidSurfaceShader.use();
        
        // render depth image on screen
        glBindVertexArray(VAOquad);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depth.ID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // use skybox Shader
        skyboxShader.use();
        glDepthFunc(GL_LEQUAL);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // render a skybox
        glBindVertexArray(VAOskybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.textureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, DirectionalLight* sun)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


    float t = 20.0f * deltaTime;
    
    // TODO : 
    // Arrow key : increase, decrease sun's azimuth, elevation with amount of t.
    float xoffset = 0.0f, yoffset = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        yoffset += t;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        yoffset -= t;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xoffset -= t;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xoffset += t;
    sun->processKeyboard(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
