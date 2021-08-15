export interface FeedTime {
  feedTime: number;
  feedDuration: number;
}

export interface PumpTime {
  onTime: number;
  offTime: number;
}

export interface AerationTime {
  onTime: number;
  offTime: number;
}

export interface Settings {
  useSystemTime: boolean;
  customTime?: string;
}

export interface IFeedSettings {
  feedTimes: FeedTime[];
  pumpTime: PumpTime;
  aerationTime: AerationTime;
  settings: Settings;
}
