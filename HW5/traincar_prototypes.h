//traincar_prototypes.h
//This is not a class header,
//	only a helper file with some function declarations

void PushBack(TrainCar* &head, TrainCar* current);
void DeleteAllCars(TrainCar* &head);
void TotalWeightAndCountCars(TrainCar* &head,int &total_weight,int &num_engines,int &num_freight_cars,int &num_passenger_cars,int &num_dining_cars,int &num_sleeping_cars);
float CalculateSpeed(TrainCar* &head);
float AverageDistanceToDiningCar(TrainCar* &head);
int ClosestEngineToSleeperCar(TrainCar* &head);
std::vector<TrainCar*> ShipFreight(TrainCar* &all_engines, TrainCar* &all_freight, int min_speed, int max_cars_per_train);
void Separate(TrainCar* &train1, TrainCar* &train2, TrainCar* &train3);
bool isItNull(TrainCar* &Train);
float SpeedProjection(TrainCar* &head, TrainCar* &car_to_add);
TrainCar* Remove_TrainCar(TrainCar* &carToRemove);
int count_cars(TrainCar* &head);