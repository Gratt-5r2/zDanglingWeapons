// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  float zTDangleNode::Amplitude = 1.0f;



  zTDangleNode::zTDangleNode( oCNpc* owner, zCModelNodeInst* nodeInstance ) {
    Owner            = owner;
    NodeInstance     = nodeInstance;
    SourceNodeMatrix = nodeInstance->trafo;
    DangledMatrix    = nodeInstance->trafo;
    Dangler          = SourceNodeMatrix.GetRightVector() * 100.0f;
    OldPoint         = Dangler;
  }



  void zTDangleNode::UpdateMatrix() {
    if( ogame->IsOnPause() )
      return;

    float weaponLength = 100.0f;
    bool longSword = false;
    oCItem* item = Owner->GetSlotVob( NodeInstance->protoNode->nodeName )->CastTo<oCItem>();
    if( item && IsMeleeWeapon( item ) ) {
      longSword = item->flags & ITM_FLAG_2HD_SWD;
      weaponLength = (float)item->range * 1.5f;
    }

    zMAT4 parentTrafoWorld = Owner->GetTrafoModelNodeToWorld( NodeInstance->parentNode );
    zMAT4 nodeTrafoWorld   = parentTrafoWorld * SourceNodeMatrix;
    zVEC3 point            = nodeTrafoWorld.GetTranslation() + nodeTrafoWorld.GetRightVector() * -weaponLength;
    zVEC3 pointLocal       = SourceNodeMatrix.GetTranslation() + SourceNodeMatrix.GetRightVector() * -weaponLength;

    zVEC3 motion      = point - OldPoint;
    zVEC3 motionLocal = nodeTrafoWorld.Transpose() * motion;
          OldPoint    = point;

    if( motion.Length() > 800 )
      return;

    zVEC3 IdlePosition = SourceNodeMatrix.GetRightVector() * weaponLength;

    Impulse -= motionLocal * (longSword ? 0.03f : 0.2f);
    Impulse += (IdlePosition - Dangler) * 0.025f;
    Dangler += Impulse * Amplitude * ztimer->factorMotion;

    if( Dangler.Length() > weaponLength )
      Dangler.Normalize() *= weaponLength;

    zVEC3 subdiv = Impulse * 0.08f;
    Impulse -= subdiv * Amplitude * ztimer->factorMotion;
    
    CheckMunitionSound( Impulse );

    if( Dangler[VY] > -10 ) {
      Dangler[VY] = -10;
      Impulse *= zVEC3( 0.9f, -0.7f, 0.9f );
    }

    zVEC3 direction1 = zVEC3( Dangler ).Normalize();
    zVEC3 direction2 = SourceNodeMatrix.GetRightVector();

    float angle    = Alg_AngleUnitRad( direction1, direction2 );
    zVEC3 axis     = Alg_RotationAxis( direction1, direction2 );
    zMAT4 rotation = Alg_Rotation3DRad( axis, angle );

    DangledMatrix = SourceNodeMatrix * rotation;
  }



  void zTDangleNode::ApplyMatrix() {
    NodeInstance->trafo = DangledMatrix;
  }



  bool zTDangleNode::HasWeapon() {
    oCItem* item = Owner->GetSlotVob( NodeInstance->protoNode->nodeName )->CastTo<oCItem>();
    return item != Null;
  }
}