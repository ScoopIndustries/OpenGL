#include "../Header/Shape.hpp"
#include "../Header/shader.hpp"


namespace GC_3D
{
	void Shape::DrawCube(vec3 Position, bool rotateShape, float angle, float i, Shader shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		if (rotateShape)
		{
			float Rangle = angle * i;
			model = glm::rotate(model, glm::radians(Rangle), glm::vec3(1.0f, 0.3f, 0.5f));
		}
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}