import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { IFeedSettings } from '../app/models/dto';

@Injectable({
  providedIn: 'root',
})
export class FeederService {
  readonly bseURL = '';

  getSettings(): Observable<IFeedSettings> {
    return this.http.get<IFeedSettings>(`${this.bseURL}/settings`);
  }

  saveSettings(settings: IFeedSettings): Observable<IFeedSettings> {
    return this.http.post<IFeedSettings>(`${this.bseURL}/settings`, settings);
  }

  constructor(private http: HttpClient) {}
}
