# MiniOFP

----------

## 기본 지식

1. 지구는 타원체이지만, 계산은 '완벽한 구' 로 가정한다.

- 계산속도의 이점, 오차가 0.5%미만으로 허용 범위 내

- 지구 반지름 R = 3440NM(약 6371km)인 구를 사용

- 근데 NM이 뭔데? => 지구 위도 1분 (1/60도)의 길이를 기준으로 하는 단위

- NM 은 약 1.852km

- 1kt (Knot) = 1시간에 1 NM을 가는 속도

1. 대권 거리

- 지구는 평면이 아니기 때문에 피타고라스 정리를 사용하면 오차가 크게 발생한다.

- 따라서 Harversine Formula를 이용해야 한다.

- 공식은 다음과 같다.

$$
a = sin^2(\frac{\Delta\phi}{2}) + cos(\phi_1) \cdot cos (\phi_2) \cdot sin^2(\frac{\Delta\lambda}{2})
$$

$$
c = 2 \cdot \text{atan2}(\sqrt{a}, \sqrt{1-a})
$$

$$
d = R \cdot c
$$

여기서:

- $\phi$: 위도 (Latitude, 라디안 단위)
- $\lambda$: 경도 (Longitude, 라디안 단위)
- $R$: 지구 반지름 (약 6,371 km)

----------


### 무게와 연료

1. OEW

- 빈 비행기의 무게(기체 + 승무원 + 기내식 등). 승객/화물/연료 뺀 상태

2. Payload

- 돈이 되는 무게(승객 + 화물).

3. Fuel

- 연료 무게.(**항공은 L가 아니라 kg 또는 lbs 무게로 따진다.** Why? 온도에 따른 부피 변화 때문)

4. MTOW (Maximum Take-Off Weight): 최대 이륙 중량

- 공식: 현재 이륙 중량 = OEW + Payload + Fuel <= MTOW

### 성능 데이터 (단순화 모델)

1. TAS (True Air Speed): 진대기속도. (예: 560 kts = 1시간에 560 NM 이동)

2. Fuel Flow (FF): 시간당 연료 소모량. (예: 2500 kg/hr)

