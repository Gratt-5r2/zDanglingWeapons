// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  struct zTDangleNode {
    oCNpc* Owner;
    zCModelNodeInst* NodeInstance;
    zMAT4 SourceNodeMatrix;
    zMAT4 DangledMatrix;
    zVEC3 Dangler;
    zVEC3 Impulse;
    zVEC3 OldPoint;

    zTDangleNode( oCNpc* owner, zCModelNodeInst* nodeInstance );
    void UpdateMatrix();
    void ApplyMatrix();
    bool HasWeapon();

    static float Amplitude;
  };


  static Array<string> SlotList;
  static Array<zTDangleNode*> Dangles;
}