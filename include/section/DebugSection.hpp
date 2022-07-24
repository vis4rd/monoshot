#pragma once

#include "../ui/elements/LowerNavigationBox.hpp"

// camera stuff
// glm::vec3 eye = glm::vec3(0.f, 0.f, 20.f);  // position camera at 0(),0,20)
// glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);  // position where camera is looking at
// glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);  // how camera is oriented
// glm::mat4 view_matrix = glm::lookAt(eye, center, up);

class DebugSection final : public Section
{
    public:
    inline DebugSection();

    inline void update() noexcept override;
    inline void render() noexcept override;

    private:
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
    ShaderManager& shaderManager;

    // content variables
    glm::vec3 scale = {50.f, 50.f, 50.f};
    float rotation = 0.f;
    glm::vec3 position = {0.f, 0.f, 0.f};
    glm::mat4 model_matrix = glm::identity<glm::mat4>();  // object space -> world space, model matrix
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));  // world space -> camera space
    glm::mat4 projection_matrix = glm::ortho(-1280.f / 2.f, 1280.f / 2.f, -720.f / 2.f, 720.f / 2.f, -100.f, 100.f);  // camera space -> normalized device coordinates
};

DebugSection::DebugSection()
    : Section(),
      shaderManager(ShaderManager::get())
{
    m_name = "DebugSection";
    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        []
        {
            SectionManager::get().popSection();
        });


    glCreateVertexArrays(1, &VAO);  // Create vertex array object (VAO) to avoid repeating below steps every frame


    uint32_t indices[] = {0, 1, 2, 2, 3, 0};
    glCreateBuffers(1, &EBO);  // Create ElementBufferObject
    glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(VAO, EBO);  // Bind EBO to VAO


    float vertices[] = {-1.f, -1.f, 0.f, -1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, -1.f, 0.f};
    int vbo_slot_pos = 0;
    glCreateBuffers(1, &VBO);  // Create a buffer ID
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Pass data to the buffer, specify nature of data
    glVertexArrayVertexBuffer(VAO, vbo_slot_pos, VBO, 0, 3 * sizeof(float));  // Bind VBO to VAO, describe offset and size of one element

    glVertexArrayAttribFormat(VAO, vbo_slot_pos, 3, GL_FLOAT, GL_FALSE, 0);  // Describe number of array elements in one attribute
    glVertexArrayAttribBinding(VAO, vbo_slot_pos, vbo_slot_pos);  // Bind this attribute with VAO and VBO
    glEnableVertexArrayAttrib(VAO, vbo_slot_pos);  // Enable this attribute in VAO


    float colors[] = {1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.5f, 0.5f, 0.5f, 1.f};
    uint32_t VBO2;
    int vbo_slot_col = 1;
    glCreateBuffers(1, &VBO2);
    glNamedBufferData(VBO2, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(VAO, vbo_slot_col, VBO2, 0, 4 * sizeof(float));

    glVertexArrayAttribFormat(VAO, vbo_slot_col, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, vbo_slot_col, vbo_slot_col);
    glEnableVertexArrayAttrib(VAO, vbo_slot_col);

    // So the structure looks more or less like this:
    // VAO
    //  L___VBO 1
    //       L___attribute 1
    //       L___attribute 2
    //       L___attribute ...
    //       L___attribute n
    //  L___VBO 2
    //       L___n attributes
    //  L___VBO ...
    //  L___VBO n
    //
    //
    // For position and color we can achieve it in two ways:
    // 1) One VAO, two VBOs with one attribute each
    // First VBO: one attribute for position
    // Second VBO: one attribute for color
    //
    // 2) One VAO, one VBO with two attributes
    // VBO data stored in pattern: [attr1_el0_data0 ... attr1_el0_datan, attr2_el0_data0 ... attr2_el0_datan, attr1_el1_data, attr2_el1_data, ..., attr1_eln, attr2_eln]
    // Example: v - vertex; p - position; x, y, z - position axes; c - color; r, g, b, a - color channels
    //     vbo_data = [v1x, v1y, v1z, v1r, v1g, v1b, v1a,, v2p, v2c, v3p, v3c,, v4, ..., vn]


    shaderManager.addShaderProgram("../shaders", "triangle_zoom");


    // model_matrix = glm::scale(model_matrix, scale);
    // model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    // model_matrix = glm::translate(model_matrix, position);

    // spdlog::debug("Model matrix:\n{}", util::mat4str(model_matrix));
    // spdlog::debug("View matrix:\n{}", util::mat4str(view_matrix));

    // glm::vec3 first = {vertices[0], vertices[1], vertices[2]};
    // glm::vec3 second = {vertices[6], vertices[7], vertices[8]};
    // glm::vec3 third = {vertices[12], vertices[13], vertices[14]};
    // spdlog::debug("Triangle:");
    // spdlog::debug("Object space: {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third));
    // first = model_matrix * glm::vec4(first, 1.f);
    // second = model_matrix * glm::vec4(second, 1.f);
    // third = model_matrix * glm::vec4(third, 1.f);
    // spdlog::debug("World space: {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third));
    // first = view_matrix * glm::vec4(first, 1.f);
    // second = view_matrix * glm::vec4(second, 1.f);
    // third = view_matrix * glm::vec4(third, 1.f);
    // spdlog::debug("Camera space: {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third));
    // first = projection_matrix * glm::vec4(first, 1.f);
    // second = projection_matrix * glm::vec4(second, 1.f);
    // third = projection_matrix * glm::vec4(third, 1.f);
    // spdlog::debug("Screen space: {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third));
}

void DebugSection::update() noexcept
{
    model_matrix = glm::scale(glm::mat4(1.f), scale);
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model_matrix = glm::translate(model_matrix, position);
}

void DebugSection::render() noexcept
{
    shaderManager.useShader("triangle_zoom");
    shaderManager.getShader("triangle_zoom").uploadMat4("uTransform", model_matrix, 0);
    shaderManager.getShader("triangle_zoom").uploadMat4("uProjection", projection_matrix, 1);
    shaderManager.getShader("triangle_zoom").uploadMat4("uView", view_matrix, 2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    ImGui::Begin("Variable editor");
    {
        ImGui::SliderFloat("rotation", &rotation, -360.f, 360.f, "%.0f degrees");
        ImGui::SliderFloat3("scale", reinterpret_cast<float*>(&scale), 0.f, 500.f);
        ImGui::SliderFloat3("position", reinterpret_cast<float*>(&position), -10.f, 10.f);
    }
    ImGui::End();
}
