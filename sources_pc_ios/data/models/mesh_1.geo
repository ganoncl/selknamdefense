// Source file: template_ship.max
CIwModel
{
	name "mesh_1"
	CMesh
	{
		name "mesh_1"
		scale 1.0
		CVerts
		{
			numVerts 6
			v {284.17908,-194.52402,200}
			v {284.17908,141.18982,199.99998}
			v {110.05764,141.18982,199.99998}
			v {2.14767,-18.92294,199.99998}
			v {2.14767,-121.49411,199.99998}
			v {110.05764,-194.52400,200}
		}
		CVertNorms
		{
			numVertNorms 6
			vn {-0.00000,0.00000,1.00000}
			vn {-0.00000,0.00000,1.00000}
			vn {0.00000,0.00000,1.00000}
			vn {0.00000,0.00000,1.00000}
			vn {0.00000,0.00000,1.00000}
			vn {0.00000,0.00000,1.00000}
		}
		CUVs
		{
			setID 0
			numUVs 6
			uv {0.71024,0.98582}
			uv {0.71024,0.14738}
			uv {0.27537,0.14738}
			uv {0.00586,0.54726}
			uv {0.00586,0.80343}
			uv {0.27537,0.98582}
		}
		CSurface
		{
			material "a"
			CQuads
			{
				numQuads 2
				q {3,3,3,-1,-1} {2,2,2,-1,-1} {5,5,5,-1,-1} {4,4,4,-1,-1}
				q {0,0,0,-1,-1} {5,5,5,-1,-1} {2,2,2,-1,-1} {1,1,1,-1,-1}
			}
		}
	}
}