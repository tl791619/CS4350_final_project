#include "GLViewInvisibleMaze.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "WOImGui.h" //GUI Demos also need to #include "AftrImGuiIncludes.h"
#include "AftrImGuiIncludes.h"
#include "AftrGLRendererBase.h"

//added includes
#include "WOGUILabel.h"
#include "WOSoundObject.h"
#include "AftrUtilities.h"
#include "helperFuncs.h"

//audio library includes 
#include "irrKlang.h"
#include "vectorToVec3df.h"

//PhysX includes 
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace irrklang; 
using namespace physx; 

GLViewInvisibleMaze* GLViewInvisibleMaze::New( const std::vector< std::string >& args )
{
   GLViewInvisibleMaze* glv = new GLViewInvisibleMaze( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petNVPHSYX);
   glv->onCreate();
   return glv;
}


GLViewInvisibleMaze::GLViewInvisibleMaze( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
 
   //PhysX init
   {
       pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pxAllocator, pxError);

       //PVD init
       pxPVD = PxCreatePvd(*pxFoundation);
       pxTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
       pxPVD->connect(*pxTransport, PxPvdInstrumentationFlag::eALL);

       //create physics that goes to PVD
       physics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true, pxPVD);

       //scene init 
       PxSceneDesc sceneDesc(physics->getTolerancesScale());
       sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
       pxDispatcher = PxDefaultCpuDispatcherCreate(2);
       sceneDesc.cpuDispatcher = pxDispatcher;
       sceneDesc.filterShader = PxDefaultSimulationFilterShader;

       pxScene = physics->createScene(sceneDesc);
       pxScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

       //set params for general material type
       genMaterial = physics->createMaterial(0.5f, 0.5f, 0.6f);
   }
   
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewInvisibleMaze::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewInvisibleMaze::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewInvisibleMaze::onCreate()
{
   //GLViewInvisibleMaze::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.
   
   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1

}


GLViewInvisibleMaze::~GLViewInvisibleMaze()
{
   //Implicitly calls GLView::~GLView()

    soundEngine->drop(); //delete the sound engine on shutdown 

    //release physx stuff 
    PX_RELEASE(pxScene);
    PX_RELEASE(pxDispatcher); 
    PX_RELEASE(physics);

    PxPvdTransport* transport = pxPVD->getTransport();
    pxPVD->release();
    pxPVD = NULL;
    PX_RELEASE(transport);

    PX_RELEASE(pxFoundation);


}


void GLViewInvisibleMaze::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.
                          
   //update the listener position to the camera's position
   vec3df cameraPosition = vectorToVec3df(this->getCamera()->getPosition());
   vec3df lookDirection = vectorToVec3df(this->getCamera()->getLookDirection());
   soundEngine->setListenerPosition(cameraPosition, lookDirection, vec3df(0,0,0), vec3df(0,0,1));
   

   //PVD sending information 
   pvdClient = pxScene->getScenePvdClient();
   if (pvdClient)
   {
       pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
       pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
       pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
   }

   //make things move! 
   if (runSimulation) {
       pxScene->simulate(0.05f);
       pxScene->fetchResults(true);

       //sync physx actors with WOs
       update(pxSphere, sphere);
       update(pxCube, cube);
   }

}


void GLViewInvisibleMaze::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewInvisibleMaze::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewInvisibleMaze::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewInvisibleMaze::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewInvisibleMaze::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if( key.keysym.sym == SDLK_1 )
   {

   }
}


void GLViewInvisibleMaze::onKeyUp(const SDL_KeyboardEvent& key)
{
    GLView::onKeyUp(key);
}

void GLViewInvisibleMaze::runPxSim(bool run) {
    runSimulation = run;
}

void Aftr::GLViewInvisibleMaze::loadMap()
{
    this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
    this->actorLst = new WorldList();
    this->netLst = new WorldList();

    ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
    ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
    ManagerOpenGLState::enableFrustumCulling = false;
    Axes::isVisible = true;
    this->glRenderer->isUsingShadowMapping(false); //set to TRUE to enable shadow mapping, must be using GL 3.2+

    this->cam->setPosition(20, 20, 10);
    this->cam->setCameraLookAtPoint(Vector(0,0,10));

    //initialize sound engine (default settings)
    soundEngine = createIrrKlangDevice();

    //play background ambience (looped) 
    {
        soundEngine->play2D("../mm/sounds/forest_wind_birds_cicadas.wav", true);
        soundEngine->setSoundVolume(volumeScalar);
    }
    

    //set the initial listener position and velocity 
    soundEngine->setListenerPosition(vec3df(15,15,10), vec3df(1, 0, 0), vec3df(0,0,0), vec3df(0,0,1)); 
    
    //path to collision sound effect 
    std::string soundPath(ManagerEnvironmentConfiguration::getLMM() + "/sounds/forest_wind_birds_cicadas.wav");
    const char* soundEffect = soundPath.c_str(); //need to convert path from string to const char* 

    //paths to named WO's to potentially use later 
    std::string cubePath(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
    std::string grass(ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl"); 
    std::string spherePath(ManagerEnvironmentConfiguration::getLMM() + "models/SimpleSphere/simple_sphere.obj");

    //Load skybox texture from Hummus.name 
    std::vector< std::string > skyBoxImageNames; //vector to store texture paths
    skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getLMM() + "/images/tantolunden_skybox.png");

    {
        //Create a light
        float ga = 0.1f; //Global Ambient Light level for this module
        ManagerLight::setGlobalAmbientLight(aftrColor4f(ga, ga, ga, 1.0f));
        WOLight* light = WOLight::New();
        light->isDirectionalLight(true);
        light->setPosition(Vector(0, 0, 100));
        //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
        //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
        light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({ 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD));
        light->setLabel("Light");
        worldLst->push_back(light);
    }

    //Create the SkyBox
    {
        WO* wo = WOSkyBox::New(skyBoxImageNames.at(0), this->getCameraPtrPtr());
        wo->setPosition(Vector(0, 0, 0));
        wo->setLabel("Sky Box");
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        worldLst->push_back(wo);
    }

    ////Create the infinite grass plane (the floor)
    {
        //load the grass as a aftrburner WO
        WO* wo = WO::New(grass, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
        wo->setPosition(Vector(0, 0, 0));
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        wo->upon_async_model_loaded([wo]()
            {
                ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
                grassSkin.getMultiTextureSet().at(0).setTexRepeats(5.0f);
                grassSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Color of object when it is not in any light
                grassSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f)); //Diffuse color components (ie, matte shading color of this object)
                grassSkin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Specular color component (ie, how "shiney" it is)
                grassSkin.setSpecularCoefficient(10); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
            });
        wo->setLabel("Grass");
        worldLst->push_back(wo);

        //create physx ground plane...
        PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *genMaterial);
        pxScene->addActor(*groundPlane);

    }

    //load in a cube for collision 
    {
        ISound* tempSound = soundEngine->play3D(soundEffect, vec3df(0, 0, 5), false, true, true);
        cube = WOSoundObject::New(cubePath, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT, tempSound); 
        cube->setVolume(0.5);
        cube->setMinDistance(10);
        cube->setLabel("cube"); 
        cube->setPosition(Vector(0,2,15)); 
        cube->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        WO* c = cube; 
        cube->upon_async_model_loaded([c]()
            {
                ModelMeshSkin& cubeSkin = c->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
                cubeSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Color of object when it is not in any light
                cubeSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f)); //Diffuse color components (ie, matte shading color of this object)
                cubeSkin.setSpecular(aftrColor4f(1.0f, 0.4f, 0.4f, 1.0f)); //Specular color component (ie, how "shiney" it is)
                cubeSkin.setSpecularCoefficient(300); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
            });
        worldLst->push_back(cube); 

        PxTransform t(PxVec3(0.0f, 15.0f, 2.0f));
        float halfExt = 2.0; //size of the cube model 
        pxCube = PxCreateDynamic(*physics, t, PxBoxGeometry(halfExt, halfExt, halfExt), *genMaterial, 10.0f);
        pxScene->addActor(*pxCube);

        update(pxCube, cube);
    }

    //load in sphere for a collision
    {
        ISound* tempSound = soundEngine->play3D(soundEffect, vec3df(0, 0, 5), false, true, true);
        sphere = WOSoundObject::New(spherePath, Vector(3, 3, 3), MESH_SHADING_TYPE::mstFLAT, tempSound);
        sphere->setVolume(0.5); 
        sphere->setMinDistance(10); 
        sphere->setLabel("sphere");
        sphere->setPosition(Vector(0, 0, 5));
        sphere->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        WO* s = sphere; 
        s->upon_async_model_loaded([s]()
            {
                ModelMeshSkin& sphereSkin = s->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);;
                sphereSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Color of object when it is not in any light
                sphereSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f)); //Diffuse color components (ie, matte shading color of this object)
                sphereSkin.setSpecular(aftrColor4f(0.2f, 0.2f, 0.2f, 1.0f)); //Specular color component (ie, how "shiney" it is)
                sphereSkin.setSpecularCoefficient(10); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
            });
        worldLst->push_back(sphere);

        PxTransform t(PxVec3(0.0f, 5.0f, 0.0f));
        pxSphere = PxCreateDynamic(*physics, t, PxSphereGeometry(3), *genMaterial, 10.0f);
        pxScene->addActor(*pxSphere); 

        update(pxSphere, sphere);
    }

    //Render label text onto the screen with project name 
    {
        WOGUILabel* screenLabel = WOGUILabel::New(nullptr);
        screenLabel->setText("Invisible Maze"); 
        screenLabel->setColor(0, 255, 200, 255);
        screenLabel->setFontSize(26);
        screenLabel->setPosition(Vector(0, 1, 0));
        screenLabel->setFontOrientation(FONT_ORIENTATION::foLEFT_TOP);
        screenLabel->setFontPath(ManagerEnvironmentConfiguration::getLMM() + "fonts/OCRAEXT.TTF");

        worldLst->push_back(screenLabel); //push the project label to the world list
    }


   //Make a Dear Im Gui instance via the WOImGui in the engine... This calls
   //the default Dear ImGui demo that shows all the features... To create your own,
   //inherit from WOImGui and override WOImGui::drawImGui_for_this_frame(...) (among any others you need).
   WOImGui* gui = WOImGui::New( nullptr );
   gui->setLabel( "My Gui" );
   gui->subscribe_drawImGuiWidget(
      [this, gui]() //this is a lambda, the capture clause is in [], the input argument list is in (), and the body is in {}
      {
           //add button interaction to play/pause physics simulation 
           ImGui::Text("Run/Pause Physics Simulation");
           if (ImGui::Button("Run/Pause")) {
               runPxSim(!runSimulation);
           }
           ImGui::Separator(); 
           if (ImGui::CollapsingHeader("Sound Controls")) {
               //float slider to control the overall sound engine volume 
               if (ImGui::SliderFloat("Main Volume", &volumeScalar, 0.0, 1.0)) {
                   soundEngine->setSoundVolume(volumeScalar);
               }
           }       
      } );
   this->worldLst->push_back( gui );

   createInvisibleMazeWayPoints();
}


void GLViewInvisibleMaze::createInvisibleMazeWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = false;
   WOWayPointSpherical* wayPt = WOWayPointSpherical::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}
