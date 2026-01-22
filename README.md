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
a = sin^2left(\frac{Deltaphi}{2}
ight) + cos phi_1 \cdot cos phi_2 \cdot sin^2left(\frac{Deltalambda}{2}
ight)
$$

$$
c = 2 \cdot \text{atan2}left(sqrt{a}, sqrt{1-a}
ight)
$$

$$
d = R \cdot c
$$

여기서:

- $phi$: 위도 (Latitude, 라디안 단위)
- $lambda$: 경도 (Longitude, 라디안 단위)
- $R$: 지구 반지름 (약 6,371 km)

----------
