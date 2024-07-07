#pragma once

#include "GLView.h"
#include <irrKlang.h>
#include "WOSoundObject.h"
#include "NetMessengerClient.h"
#include "PxPhysicsAPI.h" 
#include "Maze.h"
#include "MazeEventCallback.h"

namespace Aftr
{
   class Camera;

/**
   \class GLViewInvisibleMaze
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

   class GLViewInvisibleMaze : public GLView
   {
   public:
       static GLViewInvisibleMaze* New(const std::vector< std::string >& outArgs);
       virtual ~GLViewInvisibleMaze();
       virtual void updateWorld(); ///< Called once per frame
       virtual void loadMap(); ///< Called once at startup to build this module's scene
       virtual void createInvisibleMazeWayPoints();
       virtual void onResizeWindow(GLsizei width, GLsizei height);
       virtual void onMouseDown(const SDL_MouseButtonEvent& e);
       virtual void onMouseUp(const SDL_MouseButtonEvent& e);
       virtual void onMouseMove(const SDL_MouseMotionEvent& e);
       virtual void onKeyDown(const SDL_KeyboardEvent& key);
       virtual void onKeyUp(const SDL_KeyboardEvent& key); 

       //function for filter shader pair flags 
       static PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
           PxFilterObjectAttributes attributes1, PxFilterData filterData1,
           PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize); 

   protected:
       GLViewInvisibleMaze(const std::vector< std::string >& args);
       virtual void onCreate();

       //sound engine varaibles
       irrklang::ISoundEngine* soundEngine = nullptr;
       float volumeScalar = 0.5;

       //avatar and maze 
       WO* avatar = nullptr; 
       Maze* maze = nullptr; 
       std::string wallPath = "";
       void placeWallSegment(float x, float y, bool rotate); 

       bool pause = true; //controls running of physics simulation, defualt to paused 

       //PhysX stuff 
       physx::PxFoundation* pxFoundation = nullptr;
       physx::PxDefaultAllocator pxAllocator;
       physx::PxDefaultErrorCallback pxError;
       physx::PxScene* pxScene = nullptr; 
       physx::PxPhysics* physics = nullptr; 
       physx::PxDefaultCpuDispatcher* pxDispatcher = nullptr; 
       physx::PxMaterial* genMaterial = nullptr; 

       //PVD stuff 
       physx::PxPvd* pxPVD = nullptr;
       physx::PxPvdTransport* pxTransport = nullptr;
       physx::PxPvdSceneClient* pvdClient = nullptr;

       //PhysX actors 
       physx::PxRigidDynamic* pxAvatar = nullptr; 
       physx::PxRigidDynamic* pxCube = nullptr; 

       WO* cube = nullptr; 

   };

/** \} */

} //namespace Aftr