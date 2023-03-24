#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>

using namespace std;
using VertexPositions = Eigen::MatrixXd;
using Triangles = Eigen::MatrixXi;

tuple<VertexPositions, Triangles> makeTowel(double length) {
  Eigen::RowVector3d v0(-length, -1.0, 0.0);
  Eigen::RowVector3d v1(length, -1.0, 0.0);
  Eigen::RowVector3d v2(length, 1.0, 0.0);
  Eigen::RowVector3d v3(-length, 1.0, 0.0);

  Eigen::Matrix<double, 4, 3> vertexPositions(4, 3);
  vertexPositions << v0, v1, v2, v3;

  Eigen::RowVector3i triangle0{0, 1, 2};
  Eigen::RowVector3i triangle1{0, 2, 3};

  Triangles triangles(2, 3);
  triangles << triangle0, triangle1;

  return {vertexPositions, triangles};
}

int main(int argc, char *argv[]) {
  VertexPositions V;
  Triangles F;

  double towelLength = 2.0f;  // This variable will be updated by the GUI
  tie(V, F) = makeTowel(towelLength);

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;

  // Attach a menu plugin
  igl::opengl::glfw::imgui::ImGuiPlugin plugin;
  viewer.plugins.push_back(&plugin);
  igl::opengl::glfw::imgui::ImGuiMenu menu;
  plugin.widgets.push_back(&menu);

  // Draw additional windows
  menu.callback_draw_custom_window = [&]() {
    // Define next window position + size
    ImGui::SetNextWindowPos(ImVec2(180.f * menu.menu_scaling(), 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 160), ImGuiCond_FirstUseEver);
    ImGui::Begin("Towel Parameters", nullptr, ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushItemWidth(-80);
    ImGui::DragScalar("Towel Length", ImGuiDataType_Double, &towelLength, 0.01, 0, 0, "%.2f");
    ImGui::PopItemWidth();

    tie(V, F) = makeTowel(towelLength);
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    ImGui::End();
  };

  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
  viewer.launch();
}
