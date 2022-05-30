//
//  main.cpp
//  OpenGLDemo
//
//  Created by Marshal on 2022/5/27.
//

#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/glut.h>

#pragma mark --使用固定管线绘制四边形案例
GLBatch triangleBatch;

GLShaderManager shaderManager;

GLfloat blockSize = 0.1f;

//设置三角形顶点，全局可以用来更新位置
//其中数组vVert包含所有3个顶点的x,y,笛卡尔坐标对。
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize, 0.0f,
};

//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素
void ChangeSize(int w,int h)
{
    glViewport(0,0, w, h);
}

//为程序作一次性的设置
void SetupRC()
{
    //设置背影颜色
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    
    
    //批次处理，第一个参数决定着组合的类型或者规则
    //常用的为GL_TRIANGLES、GL_TRIANGLE_FAN、GL_TRIANGLE_STRIP
    //GL_TRIANGLES：每三个顶点组合成一个三角形，不够三个的会被忽略
    //GL_TRIANGLE_FAN：第一个顶点为中心店，绘制成三角形，然后下一个顶点分别与上一个和中心店组合另一个三角形，依次类推，即：每增加一个新的顶点，都是在与中心点和上一个点组合新的三角行，一般用于制作圆或者扇形
    //GL_TRIANGLE_STRIPL：与FAN类似，在组合一个三角形后，后面每新增一个顶点，都会与前两个顶点组合成一个三角新
    //第二个参数，表示顶点总数量，这是c传参的习惯，数量比实际顶点少，那就是用前面固定数量的顶点组合
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    triangleBatch.End();
}

//开始渲染

void RenderScene(void)
{
    /*
     清除缓存区对数值进行预置
     参数：指定将要清楚的缓存区的遮罩的按位或运算。
     GL_COLOR_BUFFER_BIT: 指示当前激活的用来进行颜色写入缓存区
     GL_DEPTH_BUFFER_BIT:指示深度缓存区
     GL_STENCIL_BUFFER_BIT:指示模板缓存区

     每个缓存区的清楚值根据这个缓存区的清楚值设置不同。
     
     错误：
     如果设定不是以上三个参考值，返回GL_INVALID_VALUE
     */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    //提交着色器
    triangleBatch.Draw();
    
    //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();
}

//键盘等事件响应回调
void SpecialKeys(int key, int x, int y){
    
    GLfloat stepSize = 0.025f;
    
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }

    //触碰到边界（4个边界）的处理
    
    //当正方形移动超过最左边的时候
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    //当正方形移动到最右边时
    //1.0 - blockSize * 2 = 总边长 - 正方形的边长 = 最左边点的位置
    if (blockX > (1.0 - blockSize * 2)) {
        blockX = 1.0f - blockSize * 2;
    }
    
    //当正方形移动到最下面时
    //-1.0 - blockSize * 2 = Y（负轴边界） - 正方形边长 = 最下面点的位置
    if (blockY < -1.0f + blockSize * 2 ) {
        blockY = -1.0f + blockSize * 2;
    }
    
    //当正方形移动到最上面时
    if (blockY > 1.0f) {
        blockY = 1.0f;
    }

    printf("blockX = %f\n",blockX);
    printf("blockY = %f\n",blockY);
    
    // Recalculate vertex positions
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    printf("(%f,%f)\n",vVerts[0],vVerts[1]);
    
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    printf("(%f,%f)\n",vVerts[3],vVerts[4]);
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    printf("(%f,%f)\n",vVerts[6],vVerts[7]);
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    printf("(%f,%f)\n",vVerts[9],vVerts[10]);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}

//设置多边形的初始化内容，包括一系列注册函数
int setupDefaultRectangle(int argc,char *argv[]) {
    //设置当前工作目录，针对MAC OS X
    /*
     `GLTools`函数`glSetWorkingDrectory`用来设置当前工作目录。实际上在Windows中是不必要的，因为工作目录默认就是与程序可执行执行程序相同的目录。但是在Mac OS X中，这个程序将当前工作文件夹改为应用程序捆绑包中的`/Resource`文件夹。`GLUT`的优先设定自动进行了这个中设置，但是这样中方法更加安全。
     */
    gltSetWorkingDirectory(argv[0]);
    
    
    //初始化GLUT库,这个函数只是传说命令参数并且初始化glut库
    glutInit(&argc, argv);
    
    /*
     初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
     
     --GLUT_DOUBLE`：双缓存窗口，是指绘图命令实际上是离屏缓存区执行的，然后迅速转换成窗口视图，这种方式，经常用来生成动画效果；
     --GLUT_DEPTH`：标志将一个深度缓存区分配为显示的一部分，因此我们能够执行深度测试；
     --GLUT_STENCIL`：确保我们也会有一个可用的模板缓存区。
     深度、模板测试后面会细致讲到
     */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小、窗口标题
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    
    /*
     GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
     1）为窗口改变大小而设置的一个回调函数
     2）包含OpenGL 渲染的回调函数
     */
    //注册重塑函数
    glutReshapeFunc(ChangeSize);
    //注册显示函数
    glutDisplayFunc(RenderScene);

    //注册特殊键位响应函数,当用户使用键盘按下特殊按键会相应
    glutSpecialFunc(SpecialKeys);
    
    /*
     初始化一个GLEW库,确保OpenGL API对程序完全可用。
     在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
     */
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
        
    }
    //设置我们的渲染环境
    SetupRC();
    
    glutMainLoop();
    
    return 0;
}


#pragma mark --直接绘制四边形系列
void drawRectangle() {
    //设置黑色背景
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1, 0, 0);//设置红色
    
    //设置绘图是的坐标系统，可以设置大小和正负，以更新边界位置
    //如果基本值是10，那么范围就是-10~10，而不是默认的-1~1
    //左、右、上、下、近、远
    glOrtho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
    
    //开始绘制多边形，连接多个点形成闭环，有点像coreGraphics框架的使用
    glBegin(GL_POLYGON);
    
    glVertex3f(-0.1f, -0.1f, 0);
    glVertex3f(0.1f, -0.1f, 0);
    glVertex3f(0.1f, 0.1f, 0);
    glVertex3f(-0.1f, 0.1f, 0);
    
    //结束绘制
    glEnd();
    //强制刷新缓存区，保证绘制后立即显示
    glFlush();
}

int setupDrawRectangle(int argc,char *argv[]) {
    //1.初始化一个GLUT库
    glutInit(&argc, (char **)argv);
    
    //2.创建一个窗口并制定窗口名
    glutInitWindowSize(800, 600);
    glutCreateWindow("CC_Window");
    
    //3.注册一个绘图函数，操作系统在必要时刻就会对窗体进行重绘制操作
    //它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程
    glutDisplayFunc(drawRectangle);
    
    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
    glutMainLoop();
    return 0;
}

#pragma mark --直接绘制圆形系列
void drawCircle() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1, 0, 0);
    //画圆实际上就是多边形，连接多个点形成闭环
    glBegin(GL_POLYGON);
    
    GLfloat R = 0.5f;//为圆的半径
    int n = 64;//n个边，即：n边形，画圆形只能靠边数多来解决问题
    GLfloat angle = 2 * M_PI / n;
    //如果想画椭圆，那么两个轴的R不一致即可，精准可以使用椭圆公式
    for (int i = 0; i < n; i++) {
        glVertex3f(R * cos(angle * i), R * sin(angle * i), 0);
    }
    
    glEnd();
    glFlush();
}

int setupDrawCircle(int argc,char *argv[]) {
    //1.初始化一个GLUT库
    glutInit(&argc, (char **)argv);
    
    //2.创建一个窗口并制定窗口名
    glutInitWindowSize(800, 800); //如果设置800*600,由于按照百分比设置，那么里面画的圆则不为圆形而是椭圆
    glutCreateWindow("CC_Window");
    
    //3.注册一个绘图函数，操作系统在必要时刻就会对窗体进行重绘制操作
    //它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程
    glutDisplayFunc(drawCircle);
    
    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
    glutMainLoop();
    return 0;
}

#pragma mark --绘制波状线，正弦余弦系列
void drawWave() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    
    GLfloat radio = 10;
    
    glOrtho(-radio, radio, radio, -radio, -radio, radio);
    
    //绘制坐标轴,独立线段,两个点画一条，该参数线条不会连接诶起来
    
    glBegin(GL_LINES);
    glVertex3f(-radio, 0, 0);
    glVertex3f(radio, 0, 0);
    glVertex3f(0, -radio, 0);
    glVertex3f(0, radio, 0);
    glEnd();
    
    //绘制正弦曲线,首尾不会连接起来，GL_LINE_LOOP会首尾连接起来
    glBegin(GL_LINE_STRIP);
    //每一度对应的弧度
    GLfloat identifyAngle = M_PI / 180;
    GLfloat R = 0.5f * radio;
    //默认2π一个周期，使用2个周期
    for (GLfloat i = -M_PI * 2; i < M_PI * 2; i += identifyAngle) {
        //sin(ω*x),w决定周期参数，2π/|ω|
        //这案例就是简易的从左往右递增度数，w增大可以增加周期数量
        glVertex3f(i, R * sin(2*i), 0);
    }
    glEnd();
    
    glFlush();
}

int setupDrawWave(int argc,char *argv[]) {
    //1.初始化一个GLUT库
    glutInit(&argc, (char **)argv);
    
    //2.创建一个窗口并制定窗口名
    glutInitWindowSize(800, 600);
    glutCreateWindow("CC_Window");
    
    //3.注册一个绘图函数，操作系统在必要时刻就会对窗体进行重绘制操作
    //它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程
    glutDisplayFunc(drawWave);
    
    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
    glutMainLoop();
    return 0;
}

int main(int argc,char *argv[])
{
//    return setupDefaultRectangle(argc, argv);
    
//    return setupDrawRectangle(argc, argv);
    
//    return setupDrawCircle(argc, argv);
    
    return setupDrawWave(argc, argv);
    
    return  0;
}
