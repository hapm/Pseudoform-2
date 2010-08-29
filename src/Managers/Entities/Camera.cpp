#include "Managers/Entities/Camera.hpp"

Camera::Camera(const string &entityName)
{
	LOG(FORMAT("Loading new entity `%1%` of type `%2%`", entityName % type()));

	mEntityName = entityName;

    mCamera = graphicSystem.getSceneMgr()->createCamera(entityName);

    mMove = 250;
    mRotate = 0.13;

    mNode = graphicSystem.getSceneMgr()->getRootSceneNode()->createChildSceneNode("Node:Camera_" + entityName);
    mPitchNode = mNode->createChildSceneNode("Node:CameraPitch_" + entityName);

    mPitchNode->attachObject(mCamera);
}

void Camera::loadFromFile(const string &filePath)
{
    ptree tree_handle = defaultLoader(filePath);

    mCamera->setFOVy(deg(tree_handle.get<float>("type_settings.fov", 90)));
    mCamera->setNearClipDistance(tree_handle.get<float>("type_settings.nearClip", 1));
    mCamera->setFarClipDistance(tree_handle.get<float>("type_settings.farClip", 1000));
    mCamera->setAutoAspectRatio(tree_handle.get<bool>("type_settings.autoAR", true));

	vector<string> parseStorage;
	float storage[3];
	memset(storage, 0, sizeof(float)*3);

	string argName = tree_handle.get<string>("type_settigns.lookAt", "0, 0, 0");
	if (parseArguments("lookAt", argName, storage, parseStorage))
		mCamera->lookAt(vec3(storage[0], storage[1], storage[2]));

    if (tree_handle.get<bool>("type_settings.attachVP", false))
        graphicSystem.getViewport()->setCamera(mCamera);

    string type = tree_handle.get<string>("type_settings.camType", "DONT_USE");
    if(type == "DONT_USE")
        setCameraType(Camera::DONT_USE);
    else if(type == "FREE")
        setCameraType(Camera::FREE);
    else if(type == "FIRST_PERSON")
        setCameraType(Camera::FIRST_PERSON);
    else if(type == "ATTACHED")
        setCameraType(Camera::ATTACHED);
    else
    	setCameraType(Camera::DONT_USE);
}

Ogre::Camera *Camera::handle() { return mCamera; }
Ogre::SceneNode *Camera::getPitchNode() { return mPitchNode; }

void Camera::setActive(bool state)
{
	// If false, set up base camera
	if (state)
		graphicSystem.getViewport()->setCamera(mCamera);
	else
		graphicSystem.getViewport()->setCamera(graphicSystem.getBaseCamera());
}
float Camera::getMoveStep() { return mMove; }
float Camera::getRotateStep() { return mRotate; }
void Camera::setMoveStep(float step) { mMove = step; }
void Camera::setRotateStep(float step) { mRotate = step; }

string Camera::type() { return "camera"; }
void Camera::update(float elapsed) { }

#define WIDTH  graphicSystem.getWindow()->getWidth()
#define HEIGHT graphicSystem.getWindow()->getHeight()

void Camera::setCameraType(CameraType type)
{
	mCameraType = type;

	if (type != Camera::DONT_USE)
	{
		//CONNECT(Engine::Events::KeyEvent, "KeyPressed", &Camera::onKeyPressed) //we don't need this anymore
	    CONNECT(Engine::Events::MouseMoveEvent, "MouseMoved", &Camera::onMouseMoved);
	    CONNECT0(Engine::Events::GlobalUpdateEvent, "Updated", &Camera::onUpdated);
	    CONNECT0(Engine::Events::GlobalInitEvent, "Inited", &Camera::onInited);
	}
}

void Camera::onInited()
{
	switch(mCameraType)
	{
		case Camera::FREE:

		break;
		case Camera::ATTACHED:
			// ...

		break;
		case Camera::FIRST_PERSON:
			// ...

		break;
		case Camera::DONT_USE: break;
	}
}

void Camera::onUpdated()
{
    using namespace sf;
	vec3 translation = vec3::ZERO;
    OIS::Keyboard *keyboard = inputSystem.getKeyboard();
	switch(mCameraType)
	{
		case Camera::FREE:
            if (keyboard->isKeyDown(OIS::KC_W)) //smoother movement instead of using the keyPressed-Callback method
			    translation.z -= mMove;
			else if (keyboard->isKeyDown(OIS::KC_S))
			    translation.z += mMove;
			if (keyboard->isKeyDown(OIS::KC_A))
			    translation.x -= mMove;
			else if (keyboard->isKeyDown(OIS::KC_D))
			    translation.x += mMove;
			if (keyboard->isKeyDown(OIS::KC_Q))
				translation.y += mMove;
			else if (keyboard->isKeyDown(OIS::KC_E))
				translation.y -= mMove;

            mNode->translate(mNode->getOrientation() * translation * gameApplicationConst.getElapsed());
		break;
		case Camera::ATTACHED:
			// ...

		break;
		case Camera::FIRST_PERSON:
			// ...

		break;
		case Camera::DONT_USE: break;
	}
}

void Camera::onKeyPressed(const OIS::KeyEvent &e)
{
	using namespace sf;
	vec3 translation = vec3::ZERO;

	switch(mCameraType)
	{
		case Camera::FREE:
			if (e.key == OIS::KC_W)
			    translation.z -= mMove;
			else if (e.key == OIS::KC_S)
			    translation.z += mMove;
			else if (e.key == OIS::KC_A)
			    translation.x -= mMove;
			else if (e.key == OIS::KC_D)
			    translation.x += mMove;
			else if (e.key == OIS::KC_Q)
				translation.y += mMove;
			else if (e.key == OIS::KC_E)
				translation -= mMove;

			mNode->translate(mNode->getOrientation() * translation * gameApplicationConst.getElapsed());

		break;
		case Camera::ATTACHED:
			// ...

		break;
		case Camera::FIRST_PERSON:
			// ...

		break;
		case Camera::DONT_USE: break;
	}
}

void Camera::onMouseMoved(const OIS::MouseEvent &e)
{
	switch(mCameraType)
	{
		case Camera::FREE:
			mNode->yaw(deg(-mRotate * e.state.X.rel), Ogre::Node::TS_WORLD);
			mPitchNode->pitch(deg(-mRotate * e.state.Y.rel), Ogre::Node::TS_LOCAL);

		break;
		case Camera::ATTACHED:
			// ...

		break;
		case Camera::FIRST_PERSON:
			// ...

		break;
		case Camera::DONT_USE: break;
	}
}
