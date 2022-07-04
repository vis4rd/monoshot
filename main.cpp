#include <stdio.h>

#include "include/App.hpp"
#include "include/input/Input.hpp"
#include "include/shader/ShaderManager.hpp"

int main(int, char**)
{
    App app("Hello OpenGL + ImGui!", 1280, 720);
    auto& window = app.getWindow();
    auto& io = window.getImGuiIo();

    Input input;

    //
    auto key_group = input.addGroup("main");
    input.addKeybind(
        "main",
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        [](GLFWwindow* window)
        {
            glfwSetWindowShouldClose(window, true);
        },
        window.getWindow());

    input.addKeybind("main",
        GLFW_KEY_ENTER,
        GLFW_PRESS,
        []()
        {
            std::cout << "Hooray!\n";
        });

    /* Create set of verticies of the triangle */
    float vertices[] =
        {-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f};

    /* Create vertex array object (VAO) to avoid repeating below steps every frame */
    unsigned int VAO;
    glCreateVertexArrays(1, &VAO);

    /* Create vertex buffer objects and generate them with buffer ID */
    unsigned int VBO;
    glCreateBuffers(1, &VBO);

    /* Pass our created verticies into the buffer */
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderManager shader_manager;
    shader_manager.addShaderProgram("../shaders", "triangle");

    /* Bind the buffer to the gpu */
    /* Tell OpenGL how our vertex data is formatted in the buffer */
    glEnableVertexArrayAttrib(VAO, 0);  // pos
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 6 * sizeof(GLfloat));

    glEnableVertexArrayAttrib(VAO, 1);  // color
    glVertexArrayAttribBinding(VAO, 1, 1);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 1, VBO, 3 * sizeof(GLfloat), 6 * sizeof(GLfloat));

    shader_manager.useShader("triangle");
    ///

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use
    // ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application
    // (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double
    // backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL,
    // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

    // Main loop
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        /* Process all user input */
        input.processGroup(window.getWindow(), "main");

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1) Prepare the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its
        // code to learn more about Dear ImGui!).
        if(show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // 2) Prepare a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color);  // Edit 3 floats representing a color

            if(ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true when
                                         // edited/activated)
            {
                counter++;
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3) Prepare another simple window.
        if(show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);  // Pass a pointer to our bool variable
                                                                   // (the window will have a closing button
                                                                   // that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if(ImGui::Button("Close Me"))
            {
                show_another_window = false;
            }
            ImGui::End();
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window.getWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste
        // this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window.getWindow());

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
        // inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or
        // clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or
        // clear/overwrite your copy of the keyboard data. Generally you may always pass all inputs to dear imgui, and
        // hide them from your application based on those two flags.
        glfwPollEvents();
    }

    return 0;
}
