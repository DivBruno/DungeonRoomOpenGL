#include "Mesh.h"
#include "tiny_obj_loader.h"

int main(){
    glfwInit();

    Set_version_profile(3, 3);

    GLFWwindow *window = glfwCreateWindow(w, h, "OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, w, h);

    Texture textures[]{
        Texture ("resource/textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture ("resource/textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
        
    };

    
    Shader shader_program("resource/shaders/default.vert", 
                          "resource/shaders/default.frag");
    Shader pokeballShader("resource/shaders/pokeball.vert", "resource/shaders/pokeball.frag");
    std::vector<Vertex> verts;
    std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));

    for (int i = 0; i < 24; i++)
        {
        Vertex v;

        v.position = glm::vec3(
            vertices[i * 11 + 0],
            vertices[i * 11 + 1],
            vertices[i * 11 + 2]
        );

        v.color = glm::vec3(
            vertices[i * 11 + 3],
            vertices[i * 11 + 4],
            vertices[i * 11 + 5]
        );

        v.texUV = glm::vec2(
            vertices[i * 11 + 6],
            vertices[i * 11 + 7]
        );

        v.normal = glm::vec3(
            vertices[i * 11 + 8],
            vertices[i * 11 + 9],
            vertices[i * 11 + 10]
        );

        verts.push_back(v);
    }
    std::vector <Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));
   
    Mesh floor(verts, inds, texs);

    Shader light_shader("resource/shaders/light.vert", 
                        "resource/shaders/light.frag");
    std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
    std::vector <GLuint> light_inds(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
    Mesh light(light_verts, light_inds, texs);

    // Aq tu muda a textura
    std::vector<Texture> modelTextures{
        Texture("resource/textures/planks.png",
                "diffuse",
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE)
    };
    std::vector<Texture> texMadeira{ Texture("resource/textures/contornoVidro.jpg","diffuse",0,GL_RGBA,GL_UNSIGNED_BYTE) };
    std::vector<Texture> texVidro{ Texture("resource/textures/vidro.jpg","diffuse",0,GL_RGBA,GL_UNSIGNED_BYTE) };
    Mesh myModel = LoadOBJ("resource/models/girl.obj");
    Mesh meshMadeira(verts, inds, texMadeira);
    Mesh meshVidro(verts, inds, texVidro);
    myModel.textures = modelTextures;

    glm::vec3 model_pos = glm::vec3(10.0f, 0.0f, -1.0f);
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, model_pos);
    

    glEnable(GL_DEPTH_TEST);

    Camera camera (w, h, glm::vec3(0.0f, 0.0f, 2.0f));

    float last_time = glfwGetTime();
        // ===== VETORES PARA A MESA =====
    std::vector<Texture> texBranco{ branco };
    std::vector<Texture> texVerde{ verde };

    // reutilizando o mesmo quadrado (verts e inds)
    Mesh meshBranco(verts, inds, texBranco);
    Mesh meshVerde(verts, inds, texVerde);

    // ===== CRIA TABLE =====
    Table table(
        glm::vec3(0.0f, 0.0f, -3.0f),
        meshBranco,
        meshVerde
    );

    Pokeball pokeballCenter(glm::vec3(0.0f, -0.35f, 0.0f), 0.06f);
    Pokeball pokeballLeft(glm::vec3(-0.45f, -0.35f, 0.0f), 0.060f);
    Pokeball pokeballRight(glm::vec3(0.45f, -0.35f, 0.0f), 0.060f);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        
        camera.Inputs(window, delta_time);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // ---- MODELO ----
        shader_program.Activate();
        pokeballShader.Activate();
        glUniformMatrix4fv(
            glGetUniformLocation(shader_program.ID, "model"),
                                1, GL_FALSE, glm::value_ptr(model_matrix)
        );
        

        // ---- CHÃO ----
        glUniformMatrix4fv(
            glGetUniformLocation(shader_program.ID, "model"),
                                1, GL_FALSE, glm::value_ptr(cube_model) // matriz original do chão
        );
       
        


        pokeballCenter.draw(pokeballShader,camera);
        pokeballLeft.draw(pokeballShader,camera);
        pokeballRight.draw(pokeballShader,camera);
        
        minhaJanela.draw(shader_program, camera);
        // ---- LUZ ----
        light.Draw(light_shader, camera);     
            

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_program.Delete();
    light_shader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}