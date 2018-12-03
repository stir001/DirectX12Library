#pragma once

#ifdef _WIN64

#ifdef _DEBUG

//#pragma comment(lib,"Bullet3Collision_vs2010_x64_debug.lib")
//#pragma comment(lib,"Bullet3Common_vs2010_x64_debug.lib")
//#pragma comment(lib,"Bullet3Dynamics_vs2010_x64_debug.lib")
//#pragma comment(lib,"Bullet3Geometry_vs2010_x64_debug.lib")
#pragma comment(lib,"BulletCollision_vs2010_x64_debug.lib")
#pragma comment(lib,"BulletDynamics_vs2010_x64_debug.lib")
//#pragma comment(lib,"BulletExampleBrowserLib_vs2010_x64_debug.lib")
//#pragma comment(lib,"BulletInverseDynamics_vs2010_x64_debug.lib")
//#pragma comment(lib,"BulletInverseDynamicsUtils_vs2010_x64_debug.lib")
//#pragma comment(lib,"BulletSoftBody_vs2010_x64_debug.lib")
//#pragma comment(lib,"BussIK_vs2010_x64_debug.lib")
//#pragma comment(lib,"gwen_vs2010_x64_debug.lib")
#pragma comment(lib,"LinearMath_vs2010_x64_debug.lib")

#else

//#pragma comment(lib,"Bullet3Collision_vs2010_x64_release.lib")
//#pragma comment(lib,"Bullet3Common_vs2010_x64_release.lib")
//#pragma comment(lib,"Bullet3Dynamics_vs2010_x64_release.lib")
//#pragma comment(lib,"Bullet3Geometry_vs2010_x64_release.lib")
#pragma comment(lib,"BulletCollision_vs2010_x64_release.lib")
#pragma comment(lib,"BulletDynamics_vs2010_x64_release.lib")
//#pragma comment(lib,"BulletExampleBrowserLib_vs2010_x64_release.lib")
//#pragma comment(lib,"BulletInverseDynamics_vs2010_x64_release.lib")
//#pragma comment(lib,"BulletInverseDynamicsUtils_vs2010_x64_release.lib")
//#pragma comment(lib,"BulletSoftBody_vs2010_x64_release.lib")
//#pragma comment(lib,"BussIK_vs2010_x64_release.lib")
//#pragma comment(lib,"gwen_vs2010_x64_release.lib")
#pragma comment(lib,"LinearMath_vs2010_x64_release.lib")

#endif

#elif defined _WIN32

#ifdef _DEBUG

//#pragma comment(lib,"Bullet3Collision_vs2010_debug.lib")
//#pragma comment(lib,"Bullet3Common_vs2010_debug.lib")
//#pragma comment(lib,"Bullet3Dynamics_vs2010_debug.lib")
//#pragma comment(lib,"Bullet3Geometry_vs2010_debug.lib")
#pragma comment(lib,"BulletCollision_vs2010_debug.lib")
#pragma comment(lib,"BulletDynamics_vs2010_debug.lib")
//#pragma comment(lib,"BulletExampleBrowserLib_vs2010_debug.lib")
//#pragma comment(lib,"BulletInverseDynamics_vs2010_debug.lib")
//#pragma comment(lib,"BulletInverseDynamicsUtils_vs2010_debug.lib")
//#pragma comment(lib,"BulletSoftBody_vs2010_debug.lib")
//#pragma comment(lib,"BussIK_vs2010_debug.lib")
//#pragma comment(lib,"gwen_vs2010_debug.lib")
#pragma comment(lib,"LinearMath_vs2010_debug.lib")

#else

//#pragma comment(lib,"Bullet3Collision_vs2010.lib")
//#pragma comment(lib,"Bullet3Common_vs2010.lib")
//#pragma comment(lib,"Bullet3Dynamics_vs2010.lib")
//#pragma comment(lib,"Bullet3Geometry_vs2010.lib")
#pragma comment(lib,"BulletCollision_vs2010.lib")
#pragma comment(lib,"BulletDynamics_vs2010.lib")
//#pragma comment(lib,"BulletExampleBrowserLib_vs2010.lib")
//#pragma comment(lib,"BulletInverseDynamics_vs2010.lib")
//#pragma comment(lib,"BulletInverseDynamicsUtils_vs2010.lib")
//#pragma comment(lib,"BulletSoftBody_vs2010.lib")
//#pragma comment(lib,"BussIK_vs2010.lib")
//#pragma comment(lib,"gwen_vs2010.lib")
#pragma comment(lib,"LinearMath_vs2010.lib")

#endif

#endif
