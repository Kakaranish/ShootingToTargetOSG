#include "ShootingTarget.hpp"

ShootingTarget::ShootingTarget(osg::Vec3f position, float radius, float thickness)
{
    _postion = position;
    _radius = radius;
    _thickness = thickness;

    _targetGeode = createTargetGeode();
    _stickGeode = createStickGeode();
    _platformGeode = createPlatformGeode();
    _movingElementsMatrixTransform = createMovingElementsMatrixTransform();
    _shootingTargetMatrixTransform = createShootingTargetMatrix();

    _apcb = new osg::AnimationPathCallback;
    const float timeInSeconds = 5;
    osg::ref_ptr<osg::AnimationPath> animationPath = createAnimationPath(timeInSeconds);
    _apcb->setAnimationPath(animationPath);
    _apcb->setTimeOffset(randomInt(0, 4));

    _movingElementsMatrixTransform->setUpdateCallback(_apcb.get());
}

osg::ref_ptr<osg::Geode> ShootingTarget::createTargetGeode()
{
    const float xAxisRotationAngle = osg::PI_2;
    osg::Vec3f targetPosition = osg::Vec3f(0, 0, getPlatformHeight() + getStickLength() + _radius);

    osg::ref_ptr<osg::Cylinder> blueCylinderShape = new osg::Cylinder(
        targetPosition, _radius, _thickness);
    blueCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> blueCylinder = new osg::ShapeDrawable(blueCylinderShape);
    blueCylinder->setColor(getColor(30, 144, 255, 255));

    osg::ref_ptr<osg::Cylinder> redCylinderShape = new osg::Cylinder(
        targetPosition + osg::Vec3f(0, -_thickness, 0), _radius / 3. * 2, 0.05f);
    redCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> redCylinder = new osg::ShapeDrawable(redCylinderShape);
    redCylinder->setColor(getColor(255, 0, 0));

    osg::ref_ptr<osg::Cylinder> yellowCylinderShape = new osg::Cylinder(
        targetPosition + osg::Vec3f(0, -(_thickness + 0.05f), 0), _radius / 3., 0.05f);
    yellowCylinderShape->setRotation(osg::Quat(xAxisRotationAngle, osg::X_AXIS));
    osg::ref_ptr<osg::ShapeDrawable> yellowCylinder = new osg::ShapeDrawable(yellowCylinderShape);
    yellowCylinder->setColor(getColor(255, 255, 0));

    osg::ref_ptr<osg::Geode> targetGeode = new osg::Geode;
    targetGeode->addDrawable(blueCylinder);
    targetGeode->addDrawable(redCylinder);
    targetGeode->addDrawable(yellowCylinder);

    return targetGeode;
}

osg::ref_ptr<osg::Geode> ShootingTarget::createStickGeode()
{
    float zOffset = _radius + getStickLength() - 1 * (_radius + getStickLength() / 2.f);
    osg::Vec3f stickPosition = osg::Vec3f(0, 0, getPlatformHeight() + zOffset);

    osg::ref_ptr<osg::ShapeDrawable> stickShapeDrawable = new osg::ShapeDrawable(
        new osg::Box(stickPosition, _thickness * 3, _thickness, getStickLength()));
    stickShapeDrawable->setColor(getColor(139, 69, 19));

    osg::ref_ptr<osg::Geode> stickGeode = new osg::Geode;
    stickGeode->addDrawable(stickShapeDrawable);
    return stickGeode;
}

osg::ref_ptr<osg::Geode> ShootingTarget::createPlatformGeode()
{
    osg::Vec3f position(0, 0, getPlatformHeight() / 2.);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Box> box = new osg::Box(position,
                                              getPlatformWidth(),
                                              getPlatformHeight(),
                                              getPlatformHeight());
    osg::ref_ptr<osg::ShapeDrawable> groundDrawable(new osg::ShapeDrawable(box));
    groundDrawable->setColor(getColor(169, 169, 169));
    geode->addDrawable(groundDrawable);
    return geode;
}

osg::ref_ptr<osg::MatrixTransform> ShootingTarget::createMovingElementsMatrixTransform()
{
    osg::ref_ptr<osg::MatrixTransform> movingElementsMatrixTransform =
        new osg::MatrixTransform;
    movingElementsMatrixTransform->addChild(_targetGeode);
    movingElementsMatrixTransform->addChild(_stickGeode);
    return movingElementsMatrixTransform;
}

osg::ref_ptr<osg::AnimationPath> ShootingTarget::createAnimationPath(float time)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode(osg::AnimationPath::LOOP);

    float movement = getPlatformWidth() / 2.f - getThickness();
    float totalMovement = movement * 4;
    unsigned numSamples = 100;
    float deltaTime = time / (float)numSamples;
    float deltaMovement = totalMovement / (float)numSamples;

    for (size_t i = 0; i < numSamples / 4; i++)
    {
        osg::AnimationPath::ControlPoint pos(osg::Vec3f(i * deltaMovement, 0.f, 0.f));
        path->insert(deltaTime * (float)i, pos);
    }

    float timeOffset = numSamples / 4 * deltaTime;
    for (size_t i = 0; i < numSamples / 2; i++)
    {
        osg::AnimationPath::ControlPoint pos(
            osg::Vec3f(movement - (i * deltaMovement), 0.f, 0.f));
        path->insert(timeOffset + deltaTime * (float)i, pos);
    }

    timeOffset += numSamples / 2 * deltaTime;
    for (size_t i = 0; i <= numSamples / 4; i++)
    {
        osg::AnimationPath::ControlPoint pos(
            osg::Vec3f(-1 * movement + (i * deltaMovement), 0.f, 0.f));
        path->insert(timeOffset + deltaTime * (float)i, pos);
    }

    return path.release();
}

osg::ref_ptr<osg::MatrixTransform> ShootingTarget::createShootingTargetMatrix()
{
    osg::ref_ptr<osg::MatrixTransform> shootingTargetMatrixTransform =
        new osg::MatrixTransform;
    shootingTargetMatrixTransform->setMatrix(osg::Matrix::translate(_postion));
    shootingTargetMatrixTransform->addChild(_movingElementsMatrixTransform);
    shootingTargetMatrixTransform->addChild(_platformGeode);

    return shootingTargetMatrixTransform;
}

void ShootingTarget::showInfo()
{
    osg::Vec3f targetBBCenter = _targetGeode->getBoundingBox().center();
    std::cout << "Target BB Center: " << targetBBCenter.x() << ", " << targetBBCenter.y() << ", " << targetBBCenter.z() << std::endl;

    std::cout << "Target BB: \txMin=" << _targetGeode->getBoundingBox().xMin() << "\txMax=" << _targetGeode->getBoundingBox().xMax() << std::endl;
    std::cout << "Target BB: \tyMin=" << _targetGeode->getBoundingBox().yMin() << "\tyMax=" << _targetGeode->getBoundingBox().yMax() << std::endl;
    std::cout << "Target BB: \tzMin=" << _targetGeode->getBoundingBox().zMin() << "\tzMax=" << _targetGeode->getBoundingBox().zMax() << std::endl;

    osg::Vec3f movingElementsTrans = _movingElementsMatrixTransform->getMatrix().getTrans();
    std::cout << "Moving elements trans: " << movingElementsTrans.x() << ", " << movingElementsTrans.y() << ", " << movingElementsTrans.z() << std::endl;

    osg::Vec3f shootingTargetMatrixTrans = _shootingTargetMatrixTransform->getMatrix().getTrans();
    std::cout << "Shooting target trans: " << shootingTargetMatrixTrans.x() << ", " << shootingTargetMatrixTrans.y() << ", " << shootingTargetMatrixTrans.z() << std::endl;

    osg::Vec3f targetCenter = shootingTargetMatrixTrans + movingElementsTrans + targetBBCenter;
    std::cout << "Target Center: " << targetCenter.x() << ", " << targetCenter.y() << ", " << targetCenter.z() << std::endl;
}

osg::ref_ptr<osg::MatrixTransform> ShootingTarget::getShootingTargetMatrixTransform()
{
    return _shootingTargetMatrixTransform;
}

osg::ref_ptr<osg::MatrixTransform> ShootingTarget::getMovingElementsMatrixTransform()
{
    return _movingElementsMatrixTransform;
}

osg::ref_ptr<osg::Geode> ShootingTarget::getTargetGeode()
{
    return _targetGeode;
}

osg::ref_ptr<osg::Geode> ShootingTarget::getPlatformGeode()
{
    return _platformGeode;
}

osg::ref_ptr<osg::Geode> ShootingTarget::getStickGeode()
{
    return _stickGeode;
}

osg::Vec3f ShootingTarget::getTargetCenter()
{
    return _targetGeode->getBoundingBox().center() +
           _movingElementsMatrixTransform->getMatrix().getTrans() +
           _shootingTargetMatrixTransform->getMatrix().getTrans();
}

float ShootingTarget::getTotalTargetThickness()
{
    float diff = _targetGeode->getBoundingBox().yMax() - _targetGeode->getBoundingBox().yMin();
    return abs(diff);
}

float ShootingTarget::getTargetRadius()
{
    return _radius;
}

float ShootingTarget::getPlatformWidth()
{
    return 4.f * _radius;
}

float ShootingTarget::getPlatformHeight()
{
    return 1.f / 3.f * _radius;
}

float ShootingTarget::getThickness()
{
    return _thickness;
}

float ShootingTarget::getStickLength()
{
    return _radius / 5.f * 4.f;
}