#include "Graph.h"
#include "Vector3D.h"
#include "GUIText.h"
#include "OGLLinePoint3D.h"

class OGLScatter3D : public Graph
{
	public:
		OGLScatter3D(int viewWidth, int viewHeight, Vector3D graphPos);
		~OGLScatter3D();

		void				Render();
		bool				MouseMove(int x, int y);
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);

		void				SetProjection();
		void				SetAxis();
		void				GraphInitialise();
		void				CreatePoints();
		void				UpdatePoints();
		void				CentreGraph();
		void				ParseData3E(ListHead* initialHead, int index1, int index2, int index3);
		void				ClearGraph();

	private:
		std::vector<OGLLinePoint3D*> pointVector;
		std::vector<GUIText*> xAxisVector;
		std::vector<GUIText*> yAxisVector;
		std::vector<GUIText*> zAxisVector;
		std::vector<graphItem3E> graph3EVector;

		std::vector<std::string> nameVectorX;
		std::vector<std::string> nameVectorY;
		std::vector<std::string> nameVectorZ;

		Vector3D				graphPos3D;
		Vector3D				xAxisTop;
		Vector3D				yAxisTop;
		Vector3D				zAxisTop;

		float					maxHeight;
		float					xAxisInterval;
		float					yAxisInterval;
		float					zAxisInterval;
		bool					graphInitialised = false;
		bool					pointsCreated = false;

		Vector3D				CoordConvertVec3D(Vector3D input);
		void					SetAxisLabels(ListHead* initialhead, int index1, int index2, int index3);
};