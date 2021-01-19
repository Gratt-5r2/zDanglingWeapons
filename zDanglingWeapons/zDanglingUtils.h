// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  inline float Alg_Min( const float& a, const float& b ) {
    return min( a, b );
  }

  inline float Alg_Max( const float& a, const float& b ) {
    return max( a, b );
  }

  inline bool IsMeleeWeapon( oCItem* item ) {
    return
      item->flags & ITM_FLAG_DAG ||
      item->flags & ITM_FLAG_SWD ||
      item->flags & ITM_FLAG_AXE ||
      item->flags & ITM_FLAG_2HD_SWD ||
      item->flags & ITM_FLAG_2HD_AXE;
  }

  inline uint GetDangledItemsCount() {
    uint count = 0;
    for( uint i = 0; i < Dangles.GetNum(); i++ )
      if( Dangles[i]->HasWeapon() )
        count++;

    return count;
  }

  inline void CheckMunitionSound( zVEC3 motion ) {
    static Timer timer;
    static int await = 500;

    if( GetDangledItemsCount() < 2 )
      return;

    if( motion.Length() > 5.0f && timer[0u].Await( await ) ) {
      int soundID = (rand() % 3) + 1;
      zsound->PlaySound3D( "DANGLE0" + Z soundID, player, 0, 0 );
      await = 500 + rand() % 500;
    }
  }
}