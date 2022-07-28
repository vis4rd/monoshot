#pragma once

#include "../ui/elements/LowerNavigationBox.hpp"

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
    glm::vec3 scale = {1.f, 1.f, 1.f};
    float rotation = 0.f;
    glm::vec3 position = {0.f, 0.f, 0.f};
    glm::mat4 model_matrix = glm::identity<glm::mat4>();
    PerspectiveCamera m_camera;

    float vertices[12] = {-1.f, -1.f, 0.f, -1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, -1.f, 0.f};
};

DebugSection::DebugSection()
    : Section(),
      shaderManager(ShaderManager::get()),
      m_camera(glm::vec3(0.f, 0.f, 50.f), {Window::get().getSize().first, Window::get().getSize().second})
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


    // glm::vec3 first = {vertices[0], vertices[1], vertices[2]};
    // glm::vec3 second = {vertices[3], vertices[4], vertices[5]};
    // glm::vec3 third = {vertices[6], vertices[7], vertices[8]};
    // glm::vec3 fourth = {vertices[9], vertices[10], vertices[11]};
    // spdlog::debug("Triangle:");
    // spdlog::debug("Object space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = model_matrix * glm::vec4(first, 1.f);
    // second = model_matrix * glm::vec4(second, 1.f);
    // third = model_matrix * glm::vec4(third, 1.f);
    // fourth = model_matrix * glm::vec4(fourth, 1.f);
    // spdlog::debug("World space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = m_camera.getViewMatrix() * glm::vec4(first, 1.f);
    // second = m_camera.getViewMatrix() * glm::vec4(second, 1.f);
    // third = m_camera.getViewMatrix() * glm::vec4(third, 1.f);
    // fourth = m_camera.getViewMatrix() * glm::vec4(fourth, 1.f);
    // spdlog::debug("Camera space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = m_camera.getProjectionMatrix() * glm::vec4(first, 1.f);
    // second = m_camera.getProjectionMatrix() * glm::vec4(second, 1.f);
    // third = m_camera.getProjectionMatrix() * glm::vec4(third, 1.f);
    // fourth = m_camera.getProjectionMatrix() * glm::vec4(fourth, 1.f);
    // spdlog::debug("Screen space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
}

void DebugSection::update() noexcept
{
    model_matrix = glm::translate(glm::mat4(1.f), position);
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model_matrix = glm::scale(model_matrix, scale);
}

void DebugSection::render() noexcept
{
    shaderManager.useShader("triangle_zoom");
    shaderManager.getShader("triangle_zoom").uploadMat4("uTransform", model_matrix, 0);
    shaderManager.getShader("triangle_zoom").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 1);
    shaderManager.getShader("triangle_zoom").uploadMat4("uView", m_camera.getViewMatrix(), 2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    ImGui::Begin("Section options");
    {
        static float zoom = 1.f;
        ImGui::SliderFloat("rotation", &rotation, -360.f, 360.f, "%.0f degrees");
        ImGui::SliderFloat2("scale", reinterpret_cast<float*>(&scale), 0.01f, 5.f);
        ImGui::SliderFloat2("position", reinterpret_cast<float*>(&position), -10.f, 10.f);
        if(ImGui::SliderFloat("camera zoom", &zoom, 0.1f, 40.f, "x%.1f"))
        {
            m_camera.setZoom(zoom);
        }
        if(ImGui::Button("Calculate matrices"))
        {
            glm::vec3 first = {vertices[0], vertices[1], vertices[2]};
            glm::vec3 second = {vertices[3], vertices[4], vertices[5]};
            glm::vec3 third = {vertices[6], vertices[7], vertices[8]};
            glm::vec3 fourth = {vertices[9], vertices[10], vertices[11]};
            model_matrix = glm::mat4(1.f);
            spdlog::debug("Beginning:\n{}", util::mat4str(model_matrix));
            model_matrix = glm::translate(model_matrix, position);
            spdlog::debug("After translation:\n{}", util::mat4str(model_matrix));
            model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
            spdlog::debug("After rotation:\n{}", util::mat4str(model_matrix));
            model_matrix = glm::scale(model_matrix, scale);
            spdlog::debug("After scaling:\n{}", util::mat4str(model_matrix));
        }
    }
    ImGui::End();
}
