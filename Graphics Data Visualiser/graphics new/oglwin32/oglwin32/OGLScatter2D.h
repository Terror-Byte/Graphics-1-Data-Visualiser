#include "Graph.h"
#include "GUIText.h"
#include "OGLLinePoint.h"

class OGLScatter2D : public Graph
{
	public:
		// Constructor and Destructor
								OGLScatter2D(int viewWidth, int viewHeight, Vector2D graphPos);
								~OGLScatter2D();

		// Override Methods
		void					Render();
		bool					MouseLBUp(int x, int y);
		bool					MouseLBDown(int x, int y);

		void					SetAxis();
		void					GraphInitialise();
		void					CreatePoints();
		void					UpdatePoints();
		void					CentreGraph();
		void					ParseData2E(ListHead* initialHead, int index1, int index2);
		void					ClearGraph();

	private:
		std::vector<OGLLinePoint*> pointVector;
		std::vector<GUIText*>	yTextVector;
		std::vector<GUIText*>	xTextVector;
		std::vector<graphItem2E> graph2EVector;
		std::vector<std::string> nameVectorX;
		std::vector<std::string> nameVectorY;
		Vector2D				xAxisTop;
		Vector2D				yAxisTop;

		float					maxHeight;
		float					xAxisInterval;
		float					yAxisInterval;
		float					graphHeight;
		float					graphWidth;
		bool					graphInitialised = false;
		bool					pointsCreated = false;

		void					SetAxisLabels(ListHead* initialHead, int index1, int index2);
};