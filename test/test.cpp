#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <iostream>

using namespace std;
using namespace glm;

void print(const glm::mat4& matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }    
}

int main()
{
    vec3 rotation = { 1.0f, 0.0f, 0.0f, };
    print(rotate(mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f }) *
            rotate(mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f }) * 
            rotate(mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f }));
    print(toMat4(quat(rotation)));
    print(lookAt(vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, -1.0f, 0.0f }, vec3{ 0.0f,  1.0f, 0.0f }));
}