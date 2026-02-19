#include "kic_timestamp/kic_timestamp.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  // テスト1: 秒から分への繰り上がり
  {
    KIC_Timestamp ts = TIMESTAMP(0, 1030); // 0日目 10:30
    ts.segments.second = 59;
    KIC_Timestamp_AddSec(&ts, 1);

    assert(ts.segments.second == 0);
    assert(ts.segments.hour_min == 1031); // 10:31になっていること
    printf("Test 1 (Sec -> Min) passed\n");
  }

  // テスト2: 分から時への繰り上がり
  {
    KIC_Timestamp ts = TIMESTAMP(0, 1059); // 10:59
    KIC_Timestamp_AddMin(&ts, 1);

    assert(ts.segments.hour_min == 1100); // 11:00
    assert(ts.segments.is_PM == 0);       // まだAM
    printf("Test 2 (Min -> Hour) passed\n");
  }

  // テスト3: AMからPMへの切り替わり (11:59 -> 0:00 PM)
  // ※hour_minが0~11想定の場合
  {
    KIC_Timestamp ts = TIMESTAMP(0, 1159);
    ts.segments.is_PM = 0;
    KIC_Timestamp_AddMin(&ts, 1);

    assert(ts.segments.hour_min == 0); // 0:00 (12:00 PM)
    assert(ts.segments.is_PM == 1);    // PMになっていること
    assert(ts.segments.day == 0);      // 日付は変わらない
    printf("Test 3 (AM -> PM) passed\n");
  }

  // テスト4: PMから翌日AMへの切り替わり (11:59 PM -> 0:00 AM Next Day)
  {
    KIC_Timestamp ts = TIMESTAMP(0, 1159);
    ts.segments.is_PM = 1; // PM
    KIC_Timestamp_AddMin(&ts, 1);

    assert(ts.segments.hour_min == 0); // 0:00
    assert(ts.segments.is_PM == 0);    // AMに戻る
    assert(ts.segments.day == 1);      // 日付がインクリメント
    printf("Test 4 (PM -> Next Day AM) passed\n");
  }

  // テスト5: 大量のミリ秒加算 (120,000ms = 2分)
  {
    KIC_Timestamp ts = TIMESTAMP(1, 550); // 1日目 5:50
    KIC_Timestamp_AddMs(&ts, 120000);

    assert(ts.segments.hour_min == 552); // 5:52
    assert(ts.segments.millisecond == 0);
    printf("Test 5 (Large Ms Add) passed\n");
  }

  // テスト6: 日付のループ (6日目 -> 0日目)
  {
    KIC_Timestamp ts = TIMESTAMP(6, 1159);
    ts.segments.is_PM = 1;
    KIC_Timestamp_AddHour(&ts, 1); // 1時間進めて日付を跨がせる

    assert(ts.segments.day == 0);
    printf("Test 6 (Day Loop 6->0) passed\n");
  }

  puts("\nAll timestamp_tests passed!");
  return 0;
}
