////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __RESPONSECURVE_H__
#define __RESPONSECURVE_H__

// class: ResponseCurve
class ResponseCurve
{
public:

	bool LoadCurve();
	bool SaveCurve();

	float CalculateValue(float _value);

	typedef struct Point_t
	{
		float x, y;
	} Point;

	ResponseCurve();
	virtual ~ResponseCurve();
protected:
	typedef std::vector<Point> PointList;
	PointList		m_PointList;
};

#endif
