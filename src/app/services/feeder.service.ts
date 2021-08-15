import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { IFeedSettings } from '../models/dto';

@Injectable({
  providedIn: 'root',
})
export class FeederService {
  readonly bseURL = 'http://localhost:3000';

  constructor(private http: HttpClient) {}

  getSettings(): Observable<IFeedSettings> {
    return this.http.get<IFeedSettings>(`${this.bseURL}/settings`);
  }

  saveSettings(settings: IFeedSettings): Observable<IFeedSettings> {
    return this.http.post<IFeedSettings>(`${this.bseURL}/settings`, settings);
  }
}
