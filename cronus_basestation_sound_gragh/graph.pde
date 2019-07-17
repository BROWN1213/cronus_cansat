cDataArray g_xAccel    = new cDataArray(200);
cDataArray g_yAccel    = new cDataArray(200);
cDataArray g_zAccel    = new cDataArray(200);
cGraph g_graph;         
  int graph_x=600,graph_y=400;
boolean g_enableFilter = false;  // Enables simple filter to help smooth out data.


void setupGraph(){

  g_graph= new cGraph(600, 0, 400, 200);
 
  
}

void drawGraph(){
  
  strokeWeight(1);
  fill(255, 255, 255);
  g_graph.drawGraphBox();

  textSize(15);
  strokeWeight(1.5);
  stroke(255, 0, 0);    line(graph_x+10, graph_y+20, graph_x+25, graph_y+20);
  stroke(0, 255, 0);     line(graph_x+10, graph_y+40, graph_x+25, graph_y+40);
  stroke(0, 0, 255);     line(graph_x+10, graph_y+60, graph_x+25, graph_y+60); 
  fill(0, 0, 0);
  text("ax", graph_x+35, graph_y+20);
  text("ay", graph_x+35, graph_y+40);
  text("az", graph_x+35, graph_y+60);
  
  strokeWeight(1.5);
  stroke(255, 0, 0);
  g_graph.drawLine(g_xAccel, -2, 2);
  stroke(0, 255, 0);
  g_graph.drawLine(g_yAccel, -2, 2);
  stroke(0, 0, 255);
  g_graph.drawLine(g_zAccel, -2, 2);  
}


// This class helps mangage the arrays of data I need to keep around for graphing.
class cDataArray
{
  float[] m_data;
  int m_maxSize;
  int m_startIndex = 0;
  int m_endIndex = 0;
  int m_curSize;
  
  cDataArray(int maxSize)
  {
    m_maxSize = maxSize;
    m_data = new float[maxSize];
  }
  
  void addVal(float val)
  {
    
    if (g_enableFilter && (m_curSize != 0))
    {
      int indx;
      
      if (m_endIndex == 0)
        indx = m_maxSize-1;
      else
        indx = m_endIndex - 1;
      
      m_data[m_endIndex] = getVal(indx)*.5 + val*.5;
    }
    else
    {
      m_data[m_endIndex] = val;
    }
    
    m_endIndex = (m_endIndex+1)%m_maxSize;
    if (m_curSize == m_maxSize)
    {
      m_startIndex = (m_startIndex+1)%m_maxSize;
    }
    else
    {
      m_curSize++;
    }
  }
  
  float getVal(int index)
  {
    return m_data[(m_startIndex+index)%m_maxSize];
  }
  
  int getCurSize()
  {
    return m_curSize;
  }
  
  int getMaxSize()
  {
    return m_maxSize;
  }
}

// This class takes the data and helps graph it
class cGraph
{
  float m_gWidth, m_gHeight;
  float m_gLeft, m_gBottom, m_gRight, m_gTop;
  
  cGraph(float x, float y, float w, float h)
  {
    m_gWidth     = w;
    m_gHeight    = h;
    m_gLeft      = x;
    m_gBottom    = height - y;
    m_gRight     = x + w;
    m_gTop       = height - y - h;    
  }
  
  void drawGraphBox()
  {
    stroke(0, 0, 0);
    rectMode(CORNERS);
    rect(m_gLeft, m_gBottom, m_gRight, m_gTop);

  }
  
  void drawLine(cDataArray data, float minRange, float maxRange)
  {
    float graphMultX = m_gWidth/data.getMaxSize();
    float graphMultY = m_gHeight/(maxRange-minRange);
    
    for(int i=0; i<data.getCurSize()-1; ++i)
    {
      float x0 = i*graphMultX+m_gLeft;
      float y0 = m_gBottom-((data.getVal(i)-minRange)*graphMultY);
      float x1 = (i+1)*graphMultX+m_gLeft;
      float y1 = m_gBottom-((data.getVal(i+1)-minRange)*graphMultY);
      line(x0, y0, x1, y1);
      //print(x0);print(",");print(y0);print(",");print(x1);print(",");println(y1);
    }
  }
}
